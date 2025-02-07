package ledger

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (l *Ledger) getStatementsFromTraces(trans *types.Transaction, s *types.Statement) ([]types.Statement, error) {
	statements := make([]types.Statement, 0, 20) // a high estimate of the number of statements we'll need

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

	if traces, err := l.connection.GetTracesByTransactionHash(trans.Hash.Hex(), trans); err != nil {
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

	if l.trialBalance(types.TrialBalTraceEth, &ret) {
		if ret.IsMaterial() {
			statements = append(statements, ret)
		} else {
			logger.TestLog(true, "Tx reconciled with a zero value net amount. It's okay.")
		}
	} else {
		// TODO: BOGUS PERF
		// logger.Warn("Trace statement at", fmt.Sprintf("%d.%d", trans.BlockNumber, trans.TransactionIndex), " does not reconcile.")
		statements = append(statements, ret)
	}

	return statements, nil
}
