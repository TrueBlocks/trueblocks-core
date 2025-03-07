package ledger1

import (
	"encoding/json"
	"fmt"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/filter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ledger10"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ledger2"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/normalize"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pricing"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/topics"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

type Reconciler1 struct {
	opts          *ledger10.ReconcilerOptions
	connection    *rpc.Connection
	accountFor    base.Address
	firstBlock    base.Blknum
	lastBlock     base.Blknum
	asEther       bool
	testMode      bool
	reversed      bool
	useTraces     bool
	assetFilter   []base.Address
	names         map[base.Address]types.Name
	hasStartBlock bool
	transfers     map[blockTxKey][]ledger10.AssetTransfer
	accountLedger map[assetHolderKey]base.Wei
	ledgerAssets  map[base.Address]bool
	// correctionCounter base.Value
	// entryCounter      base.Value
	ledgers map[base.Address]Ledger
}

func (r *Reconciler1) String() string {
	bytes, _ := json.MarshalIndent(r, "", "  ")
	return string(bytes)
}

func NewReconciler(opts *ledger10.ReconcilerOptions) *Reconciler1 {
	parts := types.Custom | types.Prefund | types.Regular
	names, _ := names.LoadNamesMap(opts.Connection.Chain, parts, []string{})
	return &Reconciler1{
		opts:          opts,
		connection:    opts.Connection,
		accountFor:    opts.AccountFor,
		firstBlock:    opts.FirstBlock,
		lastBlock:     opts.LastBlock,
		asEther:       opts.AsEther,
		testMode:      opts.TestMode,
		reversed:      opts.Reversed,
		useTraces:     opts.UseTraces,
		assetFilter:   opts.AssetFilters,
		names:         names,
		hasStartBlock: false,
		transfers:     make(map[blockTxKey][]ledger10.AssetTransfer),
		accountLedger: make(map[assetHolderKey]base.Wei),
		ledgerAssets:  make(map[base.Address]bool),
		ledgers:       make(map[base.Address]Ledger),
	}
}

type Ledger struct{}

type blockTxKey struct {
	BlockNumber      base.Blknum
	TransactionIndex base.Txnum
}

type assetHolderKey struct {
	Asset  base.Address
	Holder base.Address
}

func (r *Reconciler1) GetStatements1(pos *types.AppPosition, filter *filter.AppearanceFilter, trans *types.Transaction) ([]types.Statement, error) {
	if r.connection.Store != nil {
		statementGroup := &types.StatementGroup{
			BlockNumber:      trans.BlockNumber,
			TransactionIndex: trans.TransactionIndex,
			Address:          r.accountFor,
		}
		if err := r.connection.Store.Read(statementGroup); err == nil {
			return statementGroup.Statements, nil
		}
	}

	type AAA = types.Statement

	results := make([]AAA, 0, 20)
	var err error
	if ledger10.AssetOfInterest(r.assetFilter, base.FAKE_ETH_ADDRESS) {
		accountedFor := r.accountFor

		prevBal, _ := r.connection.GetBalanceAt(r.accountFor, pos.Prev)
		if trans.BlockNumber == 0 {
			prevBal = base.ZeroWei
		}

		begBal, _ := r.connection.GetBalanceAt(r.accountFor, trans.BlockNumber-1)
		if trans.BlockNumber == 0 {
			begBal = base.ZeroWei
		}

		endBal, _ := r.connection.GetBalanceAt(r.accountFor, trans.BlockNumber)

		at := ledger10.AssetTransfer{
			AccountedFor:     accountedFor,
			Sender:           trans.From,
			Recipient:        trans.To,
			BlockNumber:      trans.BlockNumber,
			TransactionIndex: trans.TransactionIndex,
			TransactionHash:  trans.Hash,
			LogIndex:         0,
			Timestamp:        trans.Timestamp,
			Asset:            base.FAKE_ETH_ADDRESS,
			Symbol:           "WEI",
			Decimals:         18,
			SpotPrice:        0.0,
			PriceSource:      "not-priced",
			PrevBal:          *prevBal,
			BegBal:           *begBal,
			EndBal:           *endBal,
		}

		if r.asEther {
			at.Symbol = "ETH"
		}

		if trans.To.IsZero() && trans.Receipt != nil && !trans.Receipt.ContractAddress.IsZero() {
			at.Recipient = trans.Receipt.ContractAddress
		}

		if r.useTraces {
			if traceStatements, err := r.getStatementsFromTraces(pos, trans, &at); err != nil {
				if !utils.IsFuzzing() {
					logger.Warn(colors.Yellow+"Statement at ", fmt.Sprintf("%d.%d", trans.BlockNumber, trans.TransactionIndex), " does not reconcile."+colors.Off)
				}
			} else {
				results = append(results, traceStatements...)
			}

		} else {
			if at.Sender == accountedFor {
				gasUsed := new(base.Wei)
				if trans.Receipt != nil {
					gasUsed.SetUint64(uint64(trans.Receipt.GasUsed))
				}
				gasPrice := new(base.Wei).SetUint64(uint64(trans.GasPrice))
				gasOut := new(base.Wei).Mul(gasUsed, gasPrice)
				at.AmountOut = trans.Value
				at.GasOut = *gasOut
			}

			if at.Recipient == accountedFor {
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
				}
			}

			if !utils.IsFuzzing() {
				reconciled := r.trialBalance(pos, trans, &at)
				if !reconciled {
					logger.TestLog(!r.useTraces, "Trial balance failed for ", at.TransactionHash.Hex(), " need to decend into traces")
					if traceStatements, err := r.getStatementsFromTraces(pos, trans, &at); err != nil {
						logger.Warn(colors.Yellow+"Statement at ", fmt.Sprintf("%d.%d", trans.BlockNumber, trans.TransactionIndex), " does not reconcile."+colors.Off)
					} else {
						results = append(results, traceStatements...)
					}

				} else if at.IsMaterial() {
					results = append(results, at)
				}
			}
		}
	}

	if trans.Receipt != nil {
		receiptStatements := make([]types.Statement, 0, 20)
		for _, log := range trans.Receipt.Logs {
			if log.Topics[0] != topics.TransferTopic {
				continue
			}
			addrArray := []base.Address{r.accountFor}
			if filter.ApplyLogFilter(&log, addrArray) && ledger10.AssetOfInterest(r.assetFilter, log.Address) {
				normalized, err := normalize.NormalizeKnownLogs(&log)
				if err != nil {
					continue
				} else if normalized.IsNFT() {
					continue
				} else {
					sender := base.HexToAddress(normalized.Topics[1].Hex())
					recipient := base.HexToAddress(normalized.Topics[2].Hex())
					isSender, isRecipient := r.accountFor == sender, r.accountFor == recipient
					if utils.IsFuzzing() || (!isSender && !isRecipient) {
						continue
					}

					sym := normalized.Address.DefaultSymbol()
					decimals := base.Value(18)
					name := r.names[normalized.Address]
					if name.Address == normalized.Address {
						if name.Symbol != "" {
							sym = name.Symbol
						}
						if name.Decimals != 0 {
							decimals = base.Value(name.Decimals)
						}
					}

					var amountIn, amountOut base.Wei
					amount, _ := new(base.Wei).SetString(strings.ReplaceAll(normalized.Data, "0x", ""), 16)
					if amount == nil {
						amount = base.NewWei(0)
					}
					if r.accountFor == sender {
						amountOut = *amount
					}
					if r.accountFor == recipient {
						amountIn = *amount
					}
					s := AAA{
						AccountedFor:     r.accountFor,
						Sender:           sender,
						Recipient:        recipient,
						BlockNumber:      normalized.BlockNumber,
						TransactionIndex: normalized.TransactionIndex,
						LogIndex:         normalized.LogIndex,
						TransactionHash:  normalized.TransactionHash,
						Timestamp:        normalized.Timestamp,
						Asset:            normalized.Address,
						Symbol:           sym,
						Decimals:         decimals,
						SpotPrice:        0.0,
						PriceSource:      "not-priced",
						AmountIn:         amountIn,
						AmountOut:        amountOut,
					}

					pBal, err := r.connection.GetBalanceAtToken(normalized.Address, r.accountFor, pos.Prev)
					if err != nil || pBal == nil {
						continue
					}
					s.PrevBal = *pBal
					bBal, err := r.connection.GetBalanceAtToken(normalized.Address, r.accountFor, trans.BlockNumber-1)
					if err != nil || bBal == nil {
						continue
					}
					if trans.BlockNumber == 0 {
						bBal = base.NewWei(0)
					}
					s.BegBal = *bBal
					eBal, err := r.connection.GetBalanceAtToken(normalized.Address, r.accountFor, trans.BlockNumber)
					if err != nil || eBal == nil {
						continue
					}
					s.EndBal = *eBal
					reconciled := r.trialBalance(pos, trans, &s)
					if reconciled {
						id := fmt.Sprintf(" %d.%d.%d", s.BlockNumber, s.TransactionIndex, s.LogIndex)
						logger.Progress(true, colors.Green+"Transaction", id, "reconciled       "+colors.Off)
					} else {
						id := fmt.Sprintf(" %d.%d.%d", s.BlockNumber, s.TransactionIndex, s.LogIndex)
						if os.Getenv("TEST_MODE") != "true" {
							logger.Warn("Log statement at ", id, " does not reconcile.")
						}
					}

					if s.IsMaterial() {
						receiptStatements = append(receiptStatements, s)
					}
				}
			}
		}
		results = append(results, receiptStatements...)
	}

	allReconciled := true
	for _, statement := range results {
		if statement.IsMaterial() && !statement.Reconciled() {
			allReconciled = false
			break
		}
	}

	statementGroup := &types.StatementGroup{
		BlockNumber:      trans.BlockNumber,
		TransactionIndex: trans.TransactionIndex,
		Address:          r.accountFor,
		Statements:       results,
	}
	err = r.connection.Store.WriteToCache(statementGroup, walk.Cache_Statements, trans.Timestamp, allReconciled, false)
	return results, err
}

func (r *Reconciler1) getStatementsFromTraces(pos *types.AppPosition, trans *types.Transaction, s *types.Statement) ([]types.Statement, error) {
	statements := make([]types.Statement, 0, 20)

	ret := *s
	// clear all the internal accounting values. Keeps AmountIn, AmountOut and GasOut because
	// those are at the top level (both the transaction itself and trace '0' have them). We
	// skip trace '0' because it's the same as the transaction.
	// ret.AmountIn.SetUint64(0)
	ret.InternalIn.SetUint64(0)
	ret.MinerBaseRewardIn.SetUint64(0)
	ret.MinerNephewRewardIn.SetUint64(0)
	ret.MinerTxFeeIn.SetUint64(0)
	ret.MinerUncleRewardIn.SetUint64(0)
	ret.CorrectingIn.SetUint64(0)
	ret.PrefundIn.SetUint64(0)
	ret.SelfDestructIn.SetUint64(0)

	// ret.AmountOut.SetUint64(0)
	// ret.GasOut.SetUint64(0)
	ret.InternalOut.SetUint64(0)
	ret.CorrectingOut.SetUint64(0)
	ret.SelfDestructOut.SetUint64(0)

	if traces, err := r.connection.GetTracesByTransactionHash(trans.Hash.Hex(), trans); err != nil {
		return statements, err

	} else {
		// These values accumulate...so we use += instead of =
		for i, trace := range traces {
			if i == 0 {
				// the first trace is identical to the transaction itself, so we can skip it
				continue
			}

			if trace.Action.CallType == "delegatecall" && trace.Action.To != s.AccountedFor {
				// delegate calls are not included in the transaction's gas cost, so we skip them
				continue
			}

			plusEq := func(a1, a2 *base.Wei) base.Wei {
				return *a1.Add(a1, a2)
			}

			// Do not collapse, more than one of these can be true at the same time
			if trace.Action.From == s.AccountedFor {
				ret.InternalOut = plusEq(&ret.InternalOut, &trace.Action.Value)
				ret.Sender = trace.Action.From
				if trace.Action.To.IsZero() {
					if trace.Result != nil {
						ret.Recipient = trace.Result.Address
					}
				} else {
					ret.Recipient = trace.Action.To
				}
			}

			if trace.Action.To == s.AccountedFor {
				ret.InternalIn = plusEq(&ret.InternalIn, &trace.Action.Value)
				ret.Sender = trace.Action.From
				ret.Recipient = trace.Action.To
			}

			if trace.Action.SelfDestructed == s.AccountedFor {
				ret.SelfDestructOut = plusEq(&ret.SelfDestructOut, &trace.Action.Balance)
				ret.Sender = trace.Action.SelfDestructed
				if ret.Sender.IsZero() {
					ret.Sender = trace.Action.Address
				}
				ret.Recipient = trace.Action.RefundAddress
			}

			if trace.Action.RefundAddress == s.AccountedFor {
				ret.SelfDestructIn = plusEq(&ret.SelfDestructIn, &trace.Action.Balance)
				ret.Sender = trace.Action.SelfDestructed
				if ret.Sender.IsZero() {
					ret.Sender = trace.Action.Address
				}
				ret.Recipient = trace.Action.RefundAddress
			}

			if trace.Action.Address == s.AccountedFor && !trace.Action.RefundAddress.IsZero() {
				ret.SelfDestructOut = plusEq(&ret.SelfDestructOut, &trace.Action.Balance)
				// self destructed send
				ret.Sender = trace.Action.Address
				ret.Recipient = trace.Action.RefundAddress
			}

			if trace.Result != nil {
				if trace.Result.Address == s.AccountedFor {
					ret.InternalIn = plusEq(&ret.InternalIn, &trace.Action.Value)
					ret.Sender = trace.Action.From
					ret.Recipient = trace.Result.Address
				}
			}
		}
	}

	if utils.IsFuzzing() {
		statements = append(statements, ret)
		return statements, nil
	}

	reconciled := r.trialBalance(pos, trans, &ret)
	if !reconciled {
		statements = append(statements, ret)
		return statements, nil
	}

	if ret.IsMaterial() {
		statements = append(statements, ret)
	}

	return statements, nil
}

// trialBalance returns true of the reconciliation balances, false otherwise. If the statement
// does not reconcile, it tries to repair it in two ways (a) for null transfers and (b) for
// any other reason. If that works and the statement is material (money moved in some way), the
// function tries to price the asset. it then prints optional debugging information. Note that
// the statement may be modified in this function.
func (r *Reconciler1) trialBalance(pos *types.AppPosition, trans *types.Transaction, s *types.Statement) bool {
	var okay bool
	if okay = s.Reconciled(); !okay {
		if !s.IsEth() {
			if okay = correctForNullTransfer(s, trans); !okay {
				_ = r.correctForSomethingElseToken(s)
			}
		}
	}

	if s.IsMaterial() {
		s.SpotPrice, s.PriceSource, _ = pricing.PriceUsd(r.connection, s)
		if r.testMode {
			s.DebugStatement(pos)
		}
	}

	return s.Reconciled()
}

func (r *Reconciler1) correctForSomethingElseToken(s *types.Statement) bool {
	logger.TestLog(true, "Correcting token transfer for unknown income or outflow")

	s.CorrectingIn.SetUint64(0)
	s.CorrectingOut.SetUint64(0)
	s.CorrectingReason = ""
	zero := new(base.Wei).SetInt64(0)
	cmpBegBal := s.BegBalDiff().Cmp(zero)
	cmpEndBal := s.EndBalDiff().Cmp(zero)

	if cmpBegBal > 0 {
		s.CorrectingIn = *s.BegBalDiff()
		s.CorrectingReason = "begbal"
	} else if cmpBegBal < 0 {
		s.CorrectingOut = *s.BegBalDiff()
		s.CorrectingReason = "begbal"
	}

	if cmpEndBal > 0 {
		n := new(base.Wei).Add(&s.CorrectingIn, s.EndBalDiff())
		s.CorrectingIn = *n
		s.CorrectingReason += "endbal"
	} else if cmpEndBal < 0 {
		n := new(base.Wei).Add(&s.CorrectingOut, s.EndBalDiff())
		s.CorrectingOut = *n
		s.CorrectingReason += "endbal"
	}
	s.CorrectingReason = strings.Replace(s.CorrectingReason, "begbalendbal", "begbal-endbal", -1)

	return s.Reconciled()
}

func correctForNullTransfer(s *types.Statement, tx *types.Transaction) bool {
	if s.IsNullTransfer(tx) {
		logger.TestLog(true, "Correcting token transfer for a null transfer")
		amt := s.TotalIn() // use totalIn since this is the amount that was faked
		s.AmountOut = *base.ZeroWei
		s.AmountIn = *base.ZeroWei
		s.CorrectingIn = *amt
		s.CorrectingOut = *amt
		s.CorrectingReason = "null-transfer"
	} else {
		logger.TestLog(true, "Needs correction for token transfer")
	}

	return s.Reconciled()
}

// GetTransfers1 returns a statement from a given transaction
func (r *Reconciler1) GetTransfers1(pos *types.AppPosition, filter *filter.AppearanceFilter, trans *types.Transaction) ([]types.Transfer, error) {
	if r.connection.Store != nil {
		transferGroup := &types.TransferGroup{
			BlockNumber:      trans.BlockNumber,
			TransactionIndex: trans.TransactionIndex,
		}
		if err := r.connection.Store.Read(transferGroup); err == nil {
			return transferGroup.Transfers, nil
		}
	}

	var err error
	var statements []types.Statement
	if true { // !r.useTraces {
		r2 := ledger2.NewReconciler(r.opts)
		if statements, err = r2.GetStatements2(pos, filter, trans); err != nil {
			return nil, err
		}
		// } else {
		// 	if statements, err = r.GetStatements2(pos, filter, trans); err != nil {
		// 		return nil, err
		// 	}
	}

	if transfers, err := convertToTransfers(statements); err != nil {
		return nil, err
	} else {

		allReconciled := true
		for _, transfer := range transfers {
			if transfer.IsMaterial() {
				allReconciled = false
				break
			}
		}

		transfersGroup := &types.TransferGroup{
			BlockNumber:      trans.BlockNumber,
			TransactionIndex: trans.TransactionIndex,
			Transfers:        transfers,
		}
		// TODO: BOGUS Turn on caching (remove the false below) for results once we get 100% coverage
		err = r.connection.Store.WriteToCache(transfersGroup, walk.Cache_Transfers, trans.Timestamp, allReconciled, false)
		return transfers, err
	}
}

func GetTransfersFromTransaction(tx *types.Transaction) ([]ledger10.AssetTransfer, error) {
	transfers := make([]ledger10.AssetTransfer, 0, 20)
	for _, log := range tx.Receipt.Logs {
		if log.Topics[0] != topics.TransferTopic {
			continue
		}
		normalized, err := normalize.NormalizeKnownLogs(&log)
		if err != nil {
			continue
		} else if normalized.IsNFT() {
			continue
		} else {
			sender := base.HexToAddress(normalized.Topics[1].Hex())
			recipient := base.HexToAddress(normalized.Topics[2].Hex())
			amount, _ := new(base.Wei).SetString(strings.ReplaceAll(normalized.Data, "0x", ""), 16)
			if amount == nil {
				amount = base.NewWei(0)
			}
			transfers = append(transfers, ledger10.AssetTransfer{
				Sender:    sender,
				Recipient: recipient,
				AmountIn:  *amount,
				Asset:     normalized.Address,
			})
		}
	}
	return transfers, nil
}

func convertToTransfers(statements []types.Statement) ([]types.Transfer, error) {
	transfers := make([]types.Transfer, 0, len(statements)*2)
	for _, stmnt := range statements {
		t := types.Transfer{
			Asset:            stmnt.Asset,
			Holder:           stmnt.AccountedFor,
			Amount:           *stmnt.AmountNet(),
			BlockNumber:      stmnt.BlockNumber,
			TransactionIndex: stmnt.TransactionIndex,
			LogIndex:         stmnt.LogIndex,
			Decimals:         stmnt.Decimals,
		}
		transfers = append(transfers, t)
	}
	return transfers, nil
}
