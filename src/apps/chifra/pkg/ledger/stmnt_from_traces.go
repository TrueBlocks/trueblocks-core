package ledger

import (
	"math/big"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (l *Ledger) getStatementsFromTraces(conn *rpc.Connection, trans *types.SimpleTransaction, s *types.SimpleStatement) (statements []*types.SimpleStatement) {
	statements = make([]*types.SimpleStatement, 0, 20) // a high estimate of the number of statements we'll need

	ret := *s
	ret.ClearInternal()

	if traces, err := conn.GetTracesByTransactionHash(trans.Hash.Hex(), trans); err != nil {
		logger.Error(err)

	} else {
		// These values accumulate...so we use += instead of =
		for i, trace := range traces {
			if i == 0 {
				// the first trace is identical to the transaction itself, so we can skip it
				continue
			}

			plusEq := func(a1, a2 big.Int) big.Int {
				return *a1.Add(&a1, &a2)
			}

			// Do not collapse, more than one of these can be true at the same time
			if trace.Action.From == s.AccountedFor {
				ret.InternalOut = plusEq(ret.InternalOut, trace.Action.Value)
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
				ret.InternalIn = plusEq(ret.InternalIn, trace.Action.Value)
				ret.Sender = trace.Action.From
				ret.Recipient = trace.Action.To
			}

			if trace.Action.SelfDestructed == s.AccountedFor {
				ret.SelfDestructOut = plusEq(ret.SelfDestructOut, trace.Action.Balance)
				ret.Sender = trace.Action.SelfDestructed
				if ret.Sender.IsZero() {
					ret.Sender = trace.Action.Address
				}
				ret.Recipient = trace.Action.RefundAddress
			}

			if trace.Action.RefundAddress == s.AccountedFor {
				ret.SelfDestructIn = plusEq(ret.SelfDestructIn, trace.Action.Balance)
				ret.Sender = trace.Action.SelfDestructed
				if ret.Sender.IsZero() {
					ret.Sender = trace.Action.Address
				}
				ret.Recipient = trace.Action.RefundAddress
			}

			if trace.Action.Address == s.AccountedFor && !trace.Action.RefundAddress.IsZero() {
				ret.SelfDestructOut = plusEq(ret.SelfDestructOut, trace.Action.Balance)
				// self destructed send
				ret.Sender = trace.Action.Address
				ret.Recipient = trace.Action.RefundAddress
			}

			if trace.Result != nil {
				if trace.Result.Address == s.AccountedFor {
					ret.InternalIn = plusEq(ret.InternalIn, trace.Action.Value)
					ret.Sender = trace.Action.From
					ret.Recipient = trace.Result.Address
				}
			}
		}
	}

	if l.trialBalance("ETH TRACES", &ret) {
		if ret.MoneyMoved() {
			statements = append(statements, &ret)
		} else {
			logger.TestLog(true, "Tx reconciled with a zero value net amount. It's okay.")
		}
	} else {
		// TODO: BOGUS PERF
		// logger.Warn("Transaction", fmt.Sprintf("%d.%d", trans.BlockNumber, trans.TransactionIndex), "does not reconcile")
		statements = append(statements, &ret)
	}

	return
}
