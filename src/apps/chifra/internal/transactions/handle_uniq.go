package transactionsPkg

import (
	"context"
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum"
)

func (opts *TransactionsOptions) HandleUniq() (err error) {
	chain := opts.Globals.Chain
	rpcOptions := rpcClient.DefaultRpcOptions(&rpcClient.DefaultRpcOptionsSettings{
		Chain: chain,
		Opts:  opts,
	})

	// TODO: Why does this have to dirty the caller?
	// If the cache is writeable, fetch the latest block timestamp so that we never
	// cache pending blocks
	if !rpcOptions.Store.ReadOnly() {
		rpcOptions.LatestBlockTimestamp = rpcOptions.GetBlockTimestamp(chain, nil)
	}

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawAppearance], errorChan chan error) {
		procFunc := func(s *types.SimpleAppearance) error {
			modelChan <- s
			return nil
		}

		for _, rng := range opts.TransactionIds {
			txIds, err := rng.ResolveTxs(chain)
			if err != nil && !errors.Is(err, ethereum.NotFound) {
				errorChan <- err
				cancel()
			}

			for _, app := range txIds {
				bn := uint64(app.BlockNumber)
				ts := rpcOptions.GetBlockTimestamp(chain, &bn)
				addrMap := make(index.AddressBooleanMap)

				if trans, err := rpcOptions.GetTransactionByAppearance(chain, &app, true); err != nil {
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
