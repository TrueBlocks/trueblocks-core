package ledger

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (ledgers *Ledger) GetStatementsFromTraces(trans *types.SimpleTransaction, s *types.SimpleStatement) (statements []*types.SimpleStatement) {
	chain := ledgers.Chain
	statements = make([]*types.SimpleStatement, 0, 20) // a high estimate of the number of statements we'll need

	ret := *s
	ret.ClearInternal()

	if traces, err := rpcClient.GetTracesByTransactionHash(chain, trans.Hash.Hex(), trans); err != nil {
		logger.Error(err)
	} else {
		for i, trace := range traces {
			if i == 0 {
				// the first trace is identical to the transaction itself, so we can skip it
				continue
			}

			// Do not collapse, more than one of these can be true at the same time
			// interesting := false
			if trace.Action.From == s.AccountedFor {
				ret.InternalOut = trace.Action.Value
				ret.Sender = trace.Action.From
				if trace.Action.To.IsZero() {
					if trace.Result != nil {
						ret.Recipient = trace.Result.Address
					}
				} else {
					ret.Recipient = trace.Action.To
				}
				// interesting = true
			}

			if trace.Action.To == s.AccountedFor {
				ret.InternalIn = trace.Action.Value
				ret.Sender = trace.Action.From
				ret.Recipient = trace.Action.To
				// interesting = true
			}

			// if interesting
			// {
			// 	fmt.Println("--------------------------")
			// 	fmt.Println("trace:", trace)
			// 	fmt.Println("action:", trace.Action)
			// 	fmt.Println("result:", trace.Result)
			// 	fmt.Println("--------------------------")
			// }

			if trace.Action.SelfDestructed == s.AccountedFor {
				ret.Sender = trace.Action.SelfDestructed
				if ret.Sender.IsZero() {
					ret.Sender = trace.Action.Address
				}
				ret.Recipient = trace.Action.RefundAddress
				ret.SelfDestructOut = trace.Action.Balance
			}

			if trace.Action.RefundAddress == s.AccountedFor {
				ret.Sender = trace.Action.SelfDestructed
				if ret.Sender.IsZero() {
					ret.Sender = trace.Action.Address
				}
				ret.Recipient = trace.Action.RefundAddress
				ret.SelfDestructIn = trace.Action.Balance
			}

			if trace.Action.Address == s.AccountedFor && !trace.Action.RefundAddress.IsZero() {
				// self destructed send
				ret.Sender = trace.Action.Address
				ret.Recipient = trace.Action.RefundAddress
				ret.SelfDestructOut = trace.Action.Balance
			}

			if trace.Result != nil {
				if trace.Result.Address == s.AccountedFor {
					ret.Sender = trace.Action.From
					ret.Recipient = trace.Result.Address
					ret.InternalIn = trace.Action.Value
				}
			}
		}
	}

	if ledgers.TrialBalance("ETH TRACES", &ret) {
		if ret.MoneyMoved() {
			statements = append(statements, &ret)
		} else {
			logger.TestLog(true, "Tx reconciled with a zero value net amount. It's okay.")
		}
	} else {
		logger.Warn("Transaction", fmt.Sprintf("%d.%d", trans.BlockNumber, trans.TransactionIndex), "does not reconcile")
		statements = append(statements, &ret)
	}

	return
}
