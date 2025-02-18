package ledger2

import (
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/filter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/normalize"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pricing"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/topics"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// Reconciler is responsible for processing Appearances, constructing Postings
// and LedgerEntries, appending them to Ledgers, and maintaining a LedgerBook.
type Reconciler struct {
	connection  *rpc.Connection
	names       map[base.Address]types.Name
	asEther     bool
	assetFilter []base.Address

	// Typically you'd store references to RPC clients or indexers here,
	// but we'll keep it simple for demonstration.
	LedgerBook LedgerBook
	// mu         sync.Mutex
}

// NewReconciler creates a Reconciler for the specified accountedForAddress.
func NewReconciler(conn *rpc.Connection, assetFilter []base.Address, accountedForAddress base.Address, names map[base.Address]types.Name, asEth bool) Reconciler {
	return Reconciler{
		LedgerBook:  NewLedgerBook(accountedForAddress),
		connection:  conn,
		names:       names,
		asEther:     asEth,
		assetFilter: assetFilter,
	}
}

// String returns a summary of the Reconciler’s LedgerBook.
func (r *Reconciler) String() string {
	return r.LedgerBook.String()
}

func (r *Reconciler) GetStatements(pos *types.AppPosition, filter *filter.AppearanceFilter, trans *types.Transaction) ([]types.Statement, error) {
	if trans.Hash.Hex() == "0x1cdbe0fcca2ee3f9e4504f25e6f2a485835caa920496d20b10fa6241cbfdb124" {
		fmt.Println()
	}

	xfers := r.GetAssetTransfers(pos, filter, trans)
	r.ProcessTransaction(pos, trans, xfers)

	if !r.LedgerBook.IsMaterial() {
		return []types.Statement{}, nil
	}

	return r.LedgerBook.Statements()
}

func (lb *LedgerBook) IsMaterial() bool {
	if len(lb.Ledgers) == 0 {
		return false
	}

	for _, l := range lb.Ledgers {
		net := l.NetValue()
		if !net.Equal(base.ZeroWei) {
			return true
		}
	}

	return false
}

// ProcessTransaction takes a list of Appearances and their related AssetTransfers,
// converts them to Postings, and appends them into the appropriate Ledger.
func (r *Reconciler) ProcessTransaction(pos *types.AppPosition, tx *types.Transaction, allTransfers []AssetTransfer) {
	// We assume allTransfers includes every relevant AssetTransfer for the Appearances.
	// In reality, you'd fetch them from an indexer or node calls.
	//
	// We'll map an appearanceID => LedgerEntry as we go along, then store them in the
	// correct Ledger for the asset once we've built them fully.
	//
	// This is a simplistic approach; a production system might have concurrency, etc.

	// For quick lookups:
	appearanceByID := make(map[string]types.Appearance)
	app := types.Appearance{Address: r.LedgerBook.AccountedFor, BlockNumber: uint32(tx.BlockNumber), TransactionIndex: uint32(tx.TransactionIndex)}
	appID := fmt.Sprintf("%d-%d", app.BlockNumber, app.TransactionIndex)
	appearanceByID[appID] = app

	// Build ledger entries keyed by (assetAddress + appearanceID)
	// because we might have multiple assets in the same transaction appearance
	// (e.g., an ERC20 plus some ETH).
	entriesMap := make(map[string]*LedgerEntry)

	for _, at := range allTransfers {
		// Find which appearance it belongs to
		appID := fmt.Sprintf("%d-%d", at.BlockNumber, at.TransactionIndex)
		app, ok := appearanceByID[appID]
		if !ok {
			// Possibly this is not relevant to the Reconciler's address or is missing
			continue
		}

		// Build a unique key per asset + app
		key := fmt.Sprintf("%s|%s", at.AssetAddress, appID)

		// Create the LedgerEntry if needed
		entry, found := entriesMap[key]
		if !found {
			newEntry := NewLedgerEntry(appID, base.Blknum(app.BlockNumber), base.Txnum(app.TransactionIndex))
			entriesMap[key] = &newEntry
			entry = &newEntry
		}

		if file.IsTestMode() {
			logger.TestLog(true, "Start of trial balance report")
		}

		// Convert the AssetTransfer into a single Posting (some logic is simplified):
		posting := r.convertTransferToPosting(at)
		if posting.IsMaterial() {
			posting.SpotPrice, posting.PriceSource, _ = pricing.PriceUsd(r.connection, (*types.Statement)(&posting))
		}

		// logger.Info(trans.BlockNumber, trans.TransactionIndex, r)
		if file.IsTestMode() {
			(*types.Statement)(&posting).DebugStatement(pos)
		}

		entry.Postings = append(entry.Postings, posting)
	}

	// Now we have a set of ledger entries. We'll append them into the correct Ledger for each asset.
	for key, lePtr := range entriesMap {
		// key is "assetAddress|block-tx"
		assetAddress := key[:findSeparator(key)]
		ledger, found := r.LedgerBook.GetLedger(base.HexToAddress(assetAddress))
		if !found {
			ledger = NewLedger(base.HexToAddress(assetAddress))
		}
		ledger.Entries = append(ledger.Entries, *lePtr)
		r.LedgerBook.Ledgers[ledger.AssetAddress.Hex()] = ledger
	}
}

// convertTransferToPosting transforms a single AssetTransfer into a basic Posting.
func (r *Reconciler) convertTransferToPosting(at AssetTransfer) Posting {
	ret := Posting(at)

	if at.AssetType != types.TrialBalToken && at.AssetType != types.TrialBalNft {
		prevBal, _ := r.connection.GetBalanceAt(r.LedgerBook.AccountedFor, at.BlockNumberPrev)
		if at.BlockNumber == 0 {
			prevBal = new(base.Wei)
		}

		begBal, _ := r.connection.GetBalanceAt(r.LedgerBook.AccountedFor, at.BlockNumber-1)
		if at.BlockNumber == 0 {
			begBal = new(base.Wei)
		}

		endBal, _ := r.connection.GetBalanceAt(r.LedgerBook.AccountedFor, at.BlockNumber)

		ret.PrevBal = *prevBal
		ret.BegBal = *begBal
		ret.EndBal = *endBal

	} else {
		prevBal, _ := r.connection.GetBalanceAtToken(at.AssetAddress, r.LedgerBook.AccountedFor, fmt.Sprintf("0x%x", at.BlockNumberPrev))
		if at.BlockNumber == 0 {
			prevBal = new(base.Wei)
		}

		begBal, _ := r.connection.GetBalanceAtToken(at.AssetAddress, r.LedgerBook.AccountedFor, fmt.Sprintf("0x%x", at.BlockNumber-1))
		if at.BlockNumber == 0 {
			begBal = new(base.Wei)
		}
		endBal, _ := r.connection.GetBalanceAtToken(at.AssetAddress, r.LedgerBook.AccountedFor, fmt.Sprintf("0x%x", at.BlockNumber))
		ret.PrevBal = *prevBal
		ret.BegBal = *begBal
		ret.EndBal = *endBal
	}
	return ret
}

// ReconcileCheckpoint is a placeholder for a method that verifies final ledger balances
// at a given block boundary. Real logic might compare on-chain balances and create
// correcting postings if there's a mismatch.
func (r *Reconciler) ReconcileCheckpoint(block base.Blknum) {
	// Example placeholder
	_ = block
}

// findSeparator is a small helper to locate the '|' in the string.
func findSeparator(s string) int {
	for i := 0; i < len(s); i++ {
		if s[i] == '|' {
			return i
		}
	}
	return len(s)
}

// GetAssetTransfers parses a single Transaction and returns a slice of AssetTransfer
// by checking the transaction's Value, its Logs for ERC20 events, and an optional Traces field.
func (r *Reconciler) GetAssetTransfers(pos *types.AppPosition, filter *filter.AppearanceFilter, trans *types.Transaction) []AssetTransfer {
	var results []AssetTransfer

	if AssetOfInterest(r.assetFilter, base.FAKE_ETH_ADDRESS) {
		at := AssetTransfer{
			AccountedFor:     r.LedgerBook.AccountedFor,
			BlockNumber:      trans.BlockNumber,
			BlockNumberPrev:  pos.Prev,
			BlockNumberNext:  pos.Next,
			TransactionIndex: trans.TransactionIndex,
			TransactionHash:  trans.Hash,
			Timestamp:        trans.Timestamp,
			AssetAddress:     base.FAKE_ETH_ADDRESS,
			AssetSymbol:      "WEI",
			LogIndex:         0,
			Sender:           trans.From,
			Recipient:        trans.To,
			AssetType:        types.TrialBalEth,
			Decimals:         18,
			PostFirst:        pos.First,
			PostLast:         pos.Last,
		}

		if trans.To.IsZero() && trans.Receipt != nil && !trans.Receipt.ContractAddress.IsZero() {
			at.Recipient = trans.Receipt.ContractAddress
		}

		// Do not collapse. A single transaction may have many movements of money
		if r.LedgerBook.AccountedFor == at.Sender {
			gasUsed := new(base.Wei)
			if trans.Receipt != nil {
				gasUsed.SetUint64(uint64(trans.Receipt.GasUsed))
			}
			gasPrice := new(base.Wei).SetUint64(uint64(trans.GasPrice))
			gasOut := new(base.Wei).Mul(gasUsed, gasPrice)

			at.AmountOut = trans.Value
			at.GasOut = *gasOut
		}

		// Do not collapse. A single transaction may have many movements of money
		if r.LedgerBook.AccountedFor == at.Recipient {
			if at.BlockNumber == 0 {
				at.PrefundIn = trans.Value
			} else {
				if trans.Rewards != nil {
					at.MinerBaseRewardIn = trans.Rewards.Block
					at.MinerNephewRewardIn = trans.Rewards.Nephew
					at.MinerTxFeeIn = trans.Rewards.TxFee
					at.MinerUncleRewardIn = trans.Rewards.Uncle
				} else {
					at.AmountIn = trans.Value
				}
				// TODO: BOGUS PERF - WHAT ABOUT WITHDRAWALS?
			}
		}

		if r.asEther {
			at.AssetSymbol = "ETH"
		}

		/*
			if !l.useTraces && l.trial Balance(prev, next, types.TrialBalEth, &ret) {
				if ret.IsMaterial() {
					statements = append(statements, ret)
				} else {
					logger.TestLog(true, "Tx reconciled with a zero value net amount. It's okay.")
				}
			} else {
				if !l.useTraces {
					logger.TestLog(!l.useTraces, "Trial balance failed for ", ret.TransactionHash.Hex(), " need to decend into traces")
				}
				if traceStatements, err := l.getStatementsFromTraces(trans, &ret); err != nil {
					if !utils.IsFuzzing() {
						logger.Warn(colors.Yellow+"Statement at ", fmt.Sprintf("%d.%d", trans.BlockNumber, trans.TransactionIndex), " does not reconcile."+colors.Off)
					}
				} else {
					statements = append(statements, traceStatements...)
				}
			}
		*/

		if at.IsMaterial() {
			results = append(results, at)
		}
	}

	// 2) Parse logs for ERC20 or other asset transfers. This is a simplified example
	// that checks if a log looks like an ERC20 Transfer. Real logic should decode topics, etc.
	if trans.Receipt != nil {
		for i, lg := range trans.Receipt.Logs {
			if log, isNFT, err := normalize.NormalizeKnownLogs(&lg); err != nil {
				logger.Warn("Failed to normalize log", err)
				continue

			} else if isNFT {
				continue

			} else {
				addrArray := []base.Address{r.LedgerBook.AccountedFor}
				if !filter.ApplyLogFilter(log, addrArray) || !AssetOfInterest(r.assetFilter, log.Address) {
					continue
				}

				if log.Topics[0] != topics.TransferTopic {
					continue
				}

				fromAddr := base.HexToAddress(log.Topics[1].Hex())
				toAddr := base.HexToAddress(log.Topics[2].Hex())
				var amount *base.Wei
				if amount, _ = new(base.Wei).SetString(strings.Replace(log.Data, "0x", "", i), 16); amount == nil {
					amount = base.NewWei(0)
				}

				ofInterest := false

				// Do not collapse, may be both
				amountOut := *base.ZeroWei
				if r.LedgerBook.AccountedFor == fromAddr {
					amountOut = *amount
					ofInterest = true
				}

				// Do not collapse, may be both
				amountIn := *base.ZeroWei
				if r.LedgerBook.AccountedFor == toAddr {
					amountIn = *amount
					ofInterest = true
				}

				if ofInterest {

					sym := log.Address.DefaultSymbol()
					decimals := base.Value(18)
					name := r.names[log.Address]
					if name.Address == log.Address {
						if name.Symbol != "" {
							sym = name.Symbol
						}
						if name.Decimals != 0 {
							decimals = base.Value(name.Decimals)
						}
					}

					// The contract address is in log.Address, which typically is the ERC20 token.
					at := AssetTransfer{
						AccountedFor:     r.LedgerBook.AccountedFor,
						BlockNumber:      trans.BlockNumber,
						BlockNumberPrev:  pos.Prev,
						BlockNumberNext:  pos.Next,
						TransactionIndex: trans.TransactionIndex,
						TransactionHash:  trans.Hash,
						Timestamp:        trans.Timestamp,
						AssetAddress:     lg.Address,
						AssetSymbol:      sym,
						AmountIn:         amountIn,
						AmountOut:        amountOut,
						Decimals:         decimals,
						LogIndex:         lg.LogIndex,
						Sender:           fromAddr,
						Recipient:        toAddr,
						AssetType:        types.TrialBalToken,
						PostFirst:        pos.First,
						PostLast:         pos.Last,
					}

					results = append(results, at)
				}
			}
		}
	}

	// // 3) If you have internal traces, parse them for self-destructs, internal calls, etc.
	// // For each trace that shows a value transfer from A to B, create an AssetTransfer.
	// for i, tr := range trans.Traces {
	// 	// Example check if it's a simple call with a value transfer
	// 	if strings.EqualFold(tr.Type, "call") && tr.ValueWei > 0 {
	// 		// We treat it as a native coin movement
	// 		xf := AssetTransfer{
	// 			BlockNumber:      trans.BlockNumber,
	// 			TransactionIndex: trans.TransactionIndex,
	// 			AssetAddress:        "0x0",
	// 			Amount:           *base.NewWei(tr.ValueWei),
	// 			Index:            traceIndexToString(i),
	// 			FromAddress:      tr.From,
	// 			ToAddress:        tr.To,
	// 		}
	// 		results = append(results, xf)
	// 	}
	// 	// Additional logic for selfdestruct or create, etc.
	// }

	return results
}

// AssetOfInterest returns true if the asset filter is empty or the asset matches
func AssetOfInterest(filters []base.Address, needle base.Address) bool {
	if len(filters) == 0 {
		return true
	}

	for _, asset := range filters {
		if asset.Hex() == needle.Hex() {
			return true
		}
	}

	return false
}
