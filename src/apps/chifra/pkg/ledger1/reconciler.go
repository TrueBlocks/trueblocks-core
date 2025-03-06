package ledger1

import (
	"encoding/json"
	"fmt"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/filter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ledger2"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ledger4"
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
	connection        *rpc.Connection
	accountFor        base.Address
	firstBlock        base.Blknum
	lastBlock         base.Blknum
	asEther           bool
	testMode          bool
	reversed          bool
	useTraces         bool
	assetFilter       []base.Address
	names             map[base.Address]types.Name
	hasStartBlock     bool
	transfers         map[blockTxKey][]ledger4.AssetTransfer
	accountLedger     map[assetHolderKey]base.Wei
	ledgerAssets      map[base.Address]bool
	correctionCounter base.Value
	entryCounter      base.Value
	ledgers           map[base.Address]Ledger
}

func (r *Reconciler1) String() string {
	bytes, _ := json.MarshalIndent(r, "", "  ")
	return string(bytes)
}

func NewReconciler(opts *ledger4.ReconcilerOptions) *Reconciler1 {
	parts := types.Custom | types.Prefund | types.Regular
	names, _ := names.LoadNamesMap(opts.Connection.Chain, parts, []string{})
	return &Reconciler1{
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
		transfers:     make(map[blockTxKey][]ledger4.AssetTransfer),
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
	AssetAddress base.Address
	Holder       base.Address
}

func (r *Reconciler1) getAssetTransfers(pos *types.AppPosition, filter *filter.AppearanceFilter, trans *types.Transaction) ([]ledger4.AssetTransfer, error) {
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
	if ledger4.AssetOfInterest(r.assetFilter, base.FAKE_ETH_ADDRESS) {
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

		at := AAA{
			AccountedFor:     accountedFor,
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
			PostAssetType:    types.TrialBalEth,
			SpotPrice:        0.0,
			PriceSource:      "not-priced",
			PrevBal:          *prevBal,
			BegBal:           *begBal,
			EndBal:           *endBal,
			PostFirst:        pos.First,
			PostLast:         pos.Last,
		}

		if r.asEther {
			at.AssetSymbol = "ETH"
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
				reconciled := r.trialBalance(pos, types.TrialBalEth, trans, &at)
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
			if filter.ApplyLogFilter(&log, addrArray) && ledger4.AssetOfInterest(r.assetFilter, log.Address) {
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
						AssetAddress:     normalized.Address,
						AssetSymbol:      sym,
						Decimals:         decimals,
						SpotPrice:        0.0,
						PriceSource:      "not-priced",
						AmountIn:         amountIn,
						AmountOut:        amountOut,
						PostAssetType:    types.TrialBalToken,
						PostFirst:        pos.First,
						PostLast:         pos.Last,
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
					reconciled := r.trialBalance(pos, types.TrialBalToken, trans, &s)
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

func (r *Reconciler1) GetStatements(pos *types.AppPosition, filter *filter.AppearanceFilter, trans *types.Transaction) ([]types.Statement, error) {
	return r.getAssetTransfers(pos, filter, trans)
}

func (r *Reconciler1) getStatementsFromTraces(pos *types.AppPosition, trans *types.Transaction, s *types.Statement) ([]types.Statement, error) {
	statements := make([]types.Statement, 0, 20)

	ret := *s
	ret.InternalIn.SetUint64(0)
	ret.MinerBaseRewardIn.SetUint64(0)
	ret.MinerNephewRewardIn.SetUint64(0)
	ret.MinerTxFeeIn.SetUint64(0)
	ret.MinerUncleRewardIn.SetUint64(0)
	ret.CorrectingIn.SetUint64(0)
	ret.PrefundIn.SetUint64(0)
	ret.SelfDestructIn.SetUint64(0)

	ret.InternalOut.SetUint64(0)
	ret.CorrectingOut.SetUint64(0)
	ret.SelfDestructOut.SetUint64(0)

	if traces, err := r.connection.GetTracesByTransactionHash(trans.Hash.Hex(), trans); err != nil {
		return statements, err

	} else {
		for i, trace := range traces {
			if i == 0 {
				continue
			}

			if trace.Action.CallType == "delegatecall" && trace.Action.To != s.AccountedFor {
				continue
			}

			plusEq := func(a1, a2 *base.Wei) base.Wei {
				return *a1.Add(a1, a2)
			}

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

	reconciled := r.trialBalance(pos, types.TrialBalTraceEth, trans, &ret)
	if !reconciled {
		statements = append(statements, ret)
		return statements, nil
	}

	if ret.IsMaterial() {
		statements = append(statements, ret)
	}

	return statements, nil
}

func (r *Reconciler1) trialBalance(pos *types.AppPosition, reason types.TrialBalType, trans *types.Transaction, s *types.Statement) bool {
	s.PostFirst = pos.First
	s.PostLast = pos.Last
	s.PostAssetType = reason

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
		amt := s.TotalIn()
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

func (r *Reconciler1) GetTransfers(pos *types.AppPosition, filter *filter.AppearanceFilter, trans *types.Transaction) ([]types.Transfer, error) {
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
	ledgerOpts := &ledger4.ReconcilerOptions{
		Connection:   r.connection,
		AccountFor:   r.accountFor,
		AsEther:      r.asEther,
		AssetFilters: r.assetFilter,
	}
	r2 := ledger2.NewReconciler(ledgerOpts)
	if statements, err = r2.GetStatements(pos, filter, trans); err != nil {
		return nil, err
	}

	if transfers, err := types.ConvertToTransfers(statements); err != nil {
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

		err = r.connection.Store.WriteToCache(transfersGroup, walk.Cache_Transfers, trans.Timestamp, allReconciled, false)
		return transfers, err
	}
}
