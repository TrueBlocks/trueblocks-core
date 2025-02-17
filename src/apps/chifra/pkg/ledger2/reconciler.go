package ledger2

import (
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/normalize"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// Reconciler is responsible for processing Appearances, constructing Postings
// and LedgerEntries, appending them to Ledgers, and maintaining a LedgerBook.
type Reconciler struct {
	connection *rpc.Connection
	names      map[base.Address]types.Name
	asEther    bool

	// Typically you'd store references to RPC clients or indexers here,
	// but we'll keep it simple for demonstration.
	LedgerBook LedgerBook
	// mu         sync.Mutex
}

// NewReconciler creates a Reconciler for the specified accountedForAddress.
func NewReconciler(conn *rpc.Connection, accountedForAddress base.Address, names map[base.Address]types.Name, asEth bool) Reconciler {
	return Reconciler{
		LedgerBook: NewLedgerBook(accountedForAddress),
		connection: conn,
		names:      names,
		asEther:    asEth,
	}
}

// String returns a summary of the Reconciler’s LedgerBook.
func (r *Reconciler) String() string {
	return fmt.Sprintf("Reconciler for %s => %s", r.LedgerBook.AccountedFor, r.LedgerBook.String())
}

func (r *Reconciler) ProcessAppearances(appearances []types.Appearance, allTransfers []AssetTransfer) {
	// We assume allTransfers includes every relevant AssetTransfer for the Appearances.
	// In reality, you'd fetch them from an indexer or node calls.
	//
	// We'll map an appearanceID => LedgerEntry as we go along, then store them in the
	// correct Ledger for the asset once we've built them fully.
	//
	// This is a simplistic approach; a production system might have concurrency, etc.

	// For quick lookups:
	appearanceByID := make(map[string]types.Appearance)
	for _, app := range appearances {
		// Example unique ID could be "blockNum-txIndex"
		appID := fmt.Sprintf("%d-%d", app.BlockNumber, app.TransactionIndex)
		appearanceByID[appID] = app
	}

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

		// Convert the AssetTransfer into a single Posting (some logic is simplified):
		posting := r.convertTransferToPosting(at)
		entry.AppendPosting(posting)
	}

	// Now we have a set of ledger entries. We'll append them into the correct Ledger for each asset.
	for key, lePtr := range entriesMap {
		// key is "assetAddress|block-tx"
		assetAddress := key[:findSeparator(key)]
		ledger, found := r.LedgerBook.GetLedger(assetAddress)
		if !found {
			// We create a new ledger
			assetName := guessAssetName(assetAddress) // or from at.AssetName
			ledger = NewLedger(base.HexToAddress(assetAddress), assetName)
		}
		ledger.AppendEntry(*lePtr)
		r.LedgerBook.AddLedger(ledger)
	}
}

// convertTransferToPosting transforms a single AssetTransfer into a basic Posting.
func (r *Reconciler) convertTransferToPosting(at AssetTransfer) Posting {
	// Timestamp is not retrieved here; could fetch from a block info if needed.
	p := NewPosting(at.BlockNumber, at.TransactionIndex, at.Index, 0)

	// We'll decide if it's incoming or outgoing for the accountedForAddress.
	// This is oversimplified. Real logic might check "r.LedgerBook.AccountedFor"
	// to see if 'FromAddress' or 'ToAddress' matches.
	if at.FromAddress == r.LedgerBook.AccountedFor {
		// out
		p.TransferOut = at.Amount
	} else if at.ToAddress == r.LedgerBook.AccountedFor {
		// in
		p.TransferIn = at.Amount
	}

	// If there's a gas cost or internal fees, you might handle them here similarly,
	// checking whether they're relevant to the accountedForAddress.

	// Return the constructed posting
	return p
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

// guessAssetName is a placeholder that might look up a known asset address map.
func guessAssetName(addr string) string {
	if addr == "0x0" || addr == "" {
		return "ETH"
	}
	return "UnknownAsset"
}

// DeriveAssetTransfers parses a single Transaction and returns a slice of AssetTransfer
// by checking the transaction's Value, its Logs for ERC20 events, and an optional Traces field.
func DeriveAssetTransfers(accountFor base.Address, tx *types.Transaction) []AssetTransfer {
	var results []AssetTransfer

	// if false { // assetOfInterest(l.assetFilter, base.FAKE_ETH_ADDRESS) {
	/*
			// TODO: We ignore errors in the next few lines, but we should not
			prevBal, _ := l.connection.GetBalanceAt(l.accountFor, ctx.Prev())
			if trans.BlockNumber == 0 {
				prevBal = new(base.Wei)
			}
			begBal, _ := l.connection.GetBalanceAt(l.accountFor, ctx.Cur()-1)
			if trans.BlockNumber == 0 {
				begBal = new(base.Wei)
			}
			endBal, _ := l.connection.GetBalanceAt(l.accountFor, ctx.Cur())

			ret := types.Statement{
				AccountedFor:     l.accountFor,
				Sender:           trans.From,
				Recipient:        trans.To,
				BlockNumber:      trans.BlockNumber,
				TransactionIndex: trans.TransactionIndex,
				TransactionHash:  trans.Hash,
				LogIndex:         0,
				Timestamp:        trans.Timestamp,
				AssetAddress:     base.FAKE_ETH_ADDRESS,
				AssetSymbol:      "WEI",
				Decimals:         18,
				SpotPrice:        0.0,
				PriceSource:      "not-priced",
				PrevBal:          *prevBal,
				BegBal:           *begBal,
				EndBal:           *endBal,
				ReconType:        ctx.Recon(),
			}

			if trans.To.IsZero() && trans.Receipt != nil && !trans.Receipt.ContractAddress.IsZero() {
				ret.Recipient = trans.Receipt.ContractAddress
			}

			// Do not collapse. A single transaction may have many movements of money
			if l.accountFor == ret.Sender {
				gasUsed := new(base.Wei)
				if trans.Receipt != nil {
					gasUsed.SetUint64(uint64(trans.Receipt.GasUsed))
				}
				gasPrice := new(base.Wei).SetUint64(uint64(trans.GasPrice))
				gasOut := new(base.Wei).Mul(gasUsed, gasPrice)

				ret.AmountOut = trans.Value
				ret.GasOut = *gasOut
			}

			// Do not collapse. A single transaction may have many movements of money
			if l.accountFor == ret.Recipient {
				if ret.BlockNumber == 0 {
					ret.PrefundIn = trans.Value
				} else {
					if trans.Rewards != nil {
						ret.MinerBaseRewardIn = trans.Rewards.Block
						ret.MinerNephewRewardIn = trans.Rewards.Nephew
						ret.MinerTxFeeIn = trans.Rewards.TxFee
						ret.MinerUncleRewardIn = trans.Rewards.Uncle
					} else {
						ret.AmountIn = trans.Value
					}
					// TODO: BOGUS PERF - WHAT ABOUT WITHDRAWALS?
				}
			}

			if l.asEther {
				ret.AssetSymbol = "ETH"
			}

			if !l.useTraces && l.trialBalance(types.TrialBalEth, &ret) {
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
		}

	*/
	// }

	// 1) Native transfer if tx.Value is non-zero.
	if tx.Value.Cmp(base.NewWei(0)) != 0 {
		results = append(results, AssetTransfer{
			BlockNumber:      tx.BlockNumber,
			TransactionIndex: tx.TransactionIndex,
			AssetAddress:     base.FAKE_ETH_ADDRESS, // for native chain coin
			AssetName:        "ETH",                 // or another name if not Ethereum
			Amount:           tx.Value,
			Index:            "nativeVal",
			FromAddress:      tx.From,
			ToAddress:        tx.To,
		})
	}

	// 2) Parse logs for ERC20 or other asset transfers. This is a simplified example
	// that checks if a log looks like an ERC20 Transfer. Real logic should decode topics, etc.
	if tx.Receipt != nil {
		for i, lg := range tx.Receipt.Logs {
			if log, err := normalize.NormalizeTransferOrApproval(&lg); err != nil {
				continue

			} else {
				// sym := log.Address.DefaultSymbol()
				// decimals := base.Value(18)
				// name := l.names[log.Address]
				// if name.Address == log.Address {
				// 	if name.Symbol != "" {
				// 		sym = name.Symbol
				// 	}
				// 	if name.Decimals != 0 {
				// 		decimals = base.Value(name.Decimals)
				// 	}
				// }

				fromAddr := base.HexToAddress(log.Topics[1].Hex())
				toAddr := base.HexToAddress(log.Topics[2].Hex())
				var amount *base.Wei
				if amount, _ = new(base.Wei).SetString(strings.Replace(log.Data, "0x", "", -1), 16); amount == nil {
					amount = base.NewWei(0)
				}

				ofInterest := false

				// Do not collapse, may be both
				if accountFor == fromAddr {
					// amountOut = *amt
					ofInterest = true
				}

				// Do not collapse, may be both
				if accountFor == toAddr {
					// amountIn = *amt
					ofInterest = true
				}

				if ofInterest {

					// The contract address is in log.Address, which typically is the ERC20 token.
					at := AssetTransfer{
						BlockNumber:      tx.BlockNumber,
						TransactionIndex: tx.TransactionIndex,
						AssetAddress:     lg.Address,
						AssetName:        "ERC20", // you might map the address to a known symbol
						Amount:           *amount,
						Index:            logIndexToString(i),
						FromAddress:      fromAddr,
						ToAddress:        toAddr,
					}
					results = append(results, at)
				}
			}
		}
	}

	// // 3) If you have internal traces, parse them for self-destructs, internal calls, etc.
	// // For each trace that shows a value transfer from A to B, create an AssetTransfer.
	// for i, tr := range tx.Traces {
	// 	// Example check if it's a simple call with a value transfer
	// 	if strings.EqualFold(tr.Type, "call") && tr.ValueWei > 0 {
	// 		// We treat it as a native coin movement
	// 		xf := AssetTransfer{
	// 			BlockNumber:      tx.BlockNumber,
	// 			TransactionIndex: tx.TransactionIndex,
	// 			AssetAddress:     "0x0",
	// 			AssetName:        "ETH",
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

// // isLikelyErc20Transfer is a toy placeholder. Real code would check the log's Topics for the
// // Transfer signature (0xddf252ad...), decode the from/to addresses, parse the amount, etc.
// func isLikelyErc20Transfer(lg types.Log) bool {
// 	if len(lg.Topics) == 0 {
// 		return false
// 	}
// 	// For a real ERC20 Transfer, topic[0] would be the transfer signature. We'll do a naive check.
// 	return strings.HasPrefix(lg.Topics[0].Hex(), "0xddf252ad")
// }

func logIndexToString(i int) string {
	return "log_" + fmt.Sprintf("%d", i)
}

// func traceIndexToString(i int) string {
// 	return "trace_" + fmt.Sprintf("%d", i)
// }
