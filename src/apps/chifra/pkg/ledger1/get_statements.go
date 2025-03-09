package ledger1

import (
	"fmt"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ledger10"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/normalize"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/topics"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (r *Reconciler1) GetStatements1(pos *types.AppPosition, trans *types.Transaction) ([]types.Statement, error) {
	results := make([]types.Statement, 0, 20)
	if ledger10.AssetOfInterest(r.opts.AssetFilters, base.FAKE_ETH_ADDRESS) {
		s := types.Statement{
			AccountedFor:     r.opts.AccountFor,
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
		}
		if r.opts.AsEther {
			s.Symbol = "ETH"
		}
		if trans.To.IsZero() && trans.Receipt != nil && !trans.Receipt.ContractAddress.IsZero() {
			s.Recipient = trans.Receipt.ContractAddress
		}

		reconciled := false
		if !r.opts.UseTraces {
			if s.Sender == r.opts.AccountFor {
				gasUsed := new(base.Wei)
				if trans.Receipt != nil {
					gasUsed.SetUint64(uint64(trans.Receipt.GasUsed))
				}
				gasPrice := new(base.Wei).SetUint64(uint64(trans.GasPrice))
				gasOut := new(base.Wei).Mul(gasUsed, gasPrice)
				s.AmountOut = trans.Value
				s.GasOut = *gasOut
			}

			if s.Recipient == r.opts.AccountFor {
				if s.BlockNumber == 0 {
					s.PrefundIn = trans.Value
				} else {
					if trans.Rewards != nil {
						s.MinerBaseRewardIn = trans.Rewards.Block
						s.MinerNephewRewardIn = trans.Rewards.Nephew
						s.MinerTxFeeIn = trans.Rewards.TxFee
						s.MinerUncleRewardIn = trans.Rewards.Uncle
					} else {
						s.AmountIn = trans.Value
					}
				}
			}

			var err error
			if s.PrevBal, s.BegBal, s.EndBal, err = r.opts.Connection.GetReconBalances(&rpc.BalanceOptions{
				PrevAppBlk: pos.Prev,
				CurrBlk:    trans.BlockNumber,
				Asset:      s.Asset,
				Holder:     s.AccountedFor,
			}); err != nil {
				return nil, err
			}
			reconciled = r.trialBalance(pos, trans, &s)
			if reconciled && s.IsMaterial() {
				results = append(results, s)
			}
		}

		if r.opts.UseTraces || !reconciled {
			results = make([]types.Statement, 0, 20) /* reset this */
			if s, err := r.getStatementFromTraces(pos, trans, &s); err != nil {
				logger.Warn(colors.Yellow+"Statement at ", fmt.Sprintf("%d.%d", trans.BlockNumber, trans.TransactionIndex), " does not reconcile."+colors.Off)
			} else {
				var err error
				if s.PrevBal, s.BegBal, s.EndBal, err = r.opts.Connection.GetReconBalances(&rpc.BalanceOptions{
					PrevAppBlk: pos.Prev,
					CurrBlk:    trans.BlockNumber,
					Asset:      s.Asset,
					Holder:     s.AccountedFor,
				}); err != nil {
					return nil, err
				}
				_ = r.trialBalance(pos, trans, s)
				results = append(results, *s)
			}
		}
	}

	if trans.Receipt != nil {
		if statements, err := r.getStatementsFromLogs(trans.Receipt.Logs); err != nil {
			return nil, err
		} else {
			receiptStatements := make([]types.Statement, 0, len(statements))
			for _, s := range statements {
				var err error
				if s.PrevBal, s.BegBal, s.EndBal, err = r.opts.Connection.GetReconBalances(&rpc.BalanceOptions{
					PrevAppBlk: pos.Prev,
					CurrBlk:    trans.BlockNumber,
					Asset:      s.Asset,
					Holder:     s.AccountedFor,
				}); err != nil {
					return nil, err
				}
				reconciled := r.trialBalance(pos, trans, &s)

				if reconciled {
					id := fmt.Sprintf(" %d.%d.%d", s.BlockNumber, s.TransactionIndex, s.LogIndex)
					logger.Progress(true, colors.Green+"Transaction", id, "reconciled       "+colors.Off)
				} else {
					if os.Getenv("TEST_MODE") != "true" {
						id := fmt.Sprintf(" %d.%d.%d", s.BlockNumber, s.TransactionIndex, s.LogIndex)
						logger.Warn("Log statement at ", id, " does not reconcile.")
					}
				}

				// order matters
				if s.IsMaterial() {
					receiptStatements = append(receiptStatements, s)
				}
			}
			results = append(results, receiptStatements...)
		}
	}

	return results, nil
}

func (r *Reconciler1) getStatementFromTraces(pos *types.AppPosition, trans *types.Transaction, s *types.Statement) (*types.Statement, error) {

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

	if traces, err := r.opts.Connection.GetTracesByTransactionHash(trans.Hash.Hex(), trans); err != nil {
		return nil, err

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

	return &ret, nil
}

func (r *Reconciler1) getStatementsFromLogs(logs []types.Log) ([]types.Statement, error) {
	receiptStatements := make([]types.Statement, 0, 20)
	for _, log := range logs {
		if log.Topics[0] != topics.TransferTopic {
			continue
		}
		addrArray := []base.Address{r.opts.AccountFor}
		if r.opts.AppFilters.ApplyLogFilter(&log, addrArray) && ledger10.AssetOfInterest(r.opts.AssetFilters, log.Address) {
			normalized, err := normalize.NormalizeKnownLogs(&log)
			if err != nil {
				continue
			} else if normalized.IsNFT() {
				continue
			} else {
				sender := base.HexToAddress(normalized.Topics[1].Hex())
				recipient := base.HexToAddress(normalized.Topics[2].Hex())
				isSender, isRecipient := r.opts.AccountFor == sender, r.opts.AccountFor == recipient
				if !isSender && !isRecipient {
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
				if r.opts.AccountFor == sender {
					amountOut = *amount
				}
				if r.opts.AccountFor == recipient {
					amountIn = *amount
				}
				s := types.Statement{
					AccountedFor:     r.opts.AccountFor,
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
				receiptStatements = append(receiptStatements, s)
			}
		}
	}
	return receiptStatements, nil
}
