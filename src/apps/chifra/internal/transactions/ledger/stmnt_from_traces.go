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
			if trace.Action.From == s.AccountedFor {
				ret.InternalIn = trace.Action.Value
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
				ret.Sender = trace.Action.From
				ret.Recipient = trace.Action.To
				ret.InternalIn = trace.Action.Value
			}

			if trace.Result != nil && trace.Result.Address == s.AccountedFor {
				ret.Sender = trace.Action.From
				ret.Recipient = trace.Result.Address
				ret.InternalIn = trace.Action.Value
			}
		}
	}

	if ledgers.TrialBalance("ETH TRACES", &ret) {
		if len(ret.AmountNet().Bits()) == 0 {
			logger.TestLog(true, "Tx reconciled with a zero value net amount. It's okay.")
		} else {
			statements = append(statements, &ret)
		}
	} else {
		logger.Warn(ledgers.TestMode, "Transaction", fmt.Sprintf("%d.%d", trans.BlockNumber, trans.TransactionIndex), "does not reconcile")
		statements = append(statements, &ret)
	}

	return
}
