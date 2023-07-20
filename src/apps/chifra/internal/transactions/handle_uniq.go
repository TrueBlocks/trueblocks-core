package transactionsPkg

import (
	"context"
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum"
)

func (opts *TransactionsOptions) HandleUniq() (err error) {
	rpcOptions := opts.Globals.DefaultRpcOptions(nil)
	rpcOptions.TraceWriteDisabled = !opts.CacheTraces
	chain := opts.Globals.Chain

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawAppearance], errorChan chan error) {
		procFunc := func(s *types.SimpleAppearance) error {
			modelChan <- s
			return nil
		}

		for _, rng := range opts.TransactionIds {
			txIds, err := rng.ResolveTxs(opts.Globals.Chain)
			if err != nil && !errors.Is(err, ethereum.NotFound) {
				errorChan <- err
				cancel()
			}

			for _, app := range txIds {
				bn := uint64(app.BlockNumber)
				ts := rpc.GetBlockTimestamp(chain, &bn)
				addrMap := make(index.AddressBooleanMap)

				if trans, err := rpcClient.GetTransactionByAppearance(chain, &app, true, rpcOptions); err != nil {
					errorChan <- err
				} else {
					if err = index.UniqFromTransDetails(chain, procFunc, opts.Flow, trans, ts, addrMap, rpcOptions); err != nil {
						errorChan <- err
					}
				}
			}
		}
	}

	extra := map[string]interface{}{
		"uniq": true,
	}
	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}
