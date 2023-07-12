package receiptsPkg

import (
	"context"
	"fmt"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *ReceiptsOptions) HandleShowReceipts() error {
	abiCache := articulate.NewAbiCache()
	chain := opts.Globals.Chain

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawReceipt], errorChan chan error) {
		if appMap, err := identifiers.AsMap[types.SimpleReceipt](chain, opts.TransactionIds); err != nil {
			errorChan <- err
			cancel()
		} else {
			showProgress := !opts.Globals.TestMode && len(opts.Globals.File) == 0
			var bar = logger.NewBar("", showProgress, int64(len(appMap)))

			// This is called concurrently, once for each appearance
			iterCtx, iterCancel := context.WithCancel(context.Background())
			defer iterCancel()
			iterFunc := func(app identifiers.ResolvedId, value *types.SimpleReceipt) error {
				if receipt, err := rpcClient.GetTransactionReceipt(opts.Globals.Chain, rpcClient.ReceiptQuery{
					Bn:      app.BlockNumber,
					Txid:    app.TransactionIndex,
					NeedsTs: true,
				}); err != nil {
					return fmt.Errorf("transaction at %s returned an error: %w", app.String(), err)
				} else {
					if opts.Articulate {
						if err = abiCache.ArticulateReceipt(chain, &receipt); err != nil {
							errorChan <- err // continue even with an error
						}
					}
					*value = receipt
					bar.Tick()
					return nil
				}
			}

			// Set up and interate over the map calling iterFunc for each appearance
			iterErrorChan := make(chan error, 10)
			utils.IterateOverMap(iterCtx, iterErrorChan, appMap, iterFunc)
			for err := range iterErrorChan {
				// TODO: I don't really want to quit looping here. Just report the error and keep going.
				iterCancel()
				errorChan <- err
			}
			bar.Finish(true)

			// Sort the items back into an ordered array by block number
			items := make([]*types.SimpleReceipt, 0, len(appMap))
			for _, r := range appMap {
				items = append(items, r)
			}
			sort.Slice(items, func(i, j int) bool {
				if items[i].BlockNumber == items[j].BlockNumber {
					return items[i].TransactionIndex < items[j].TransactionIndex
				}
				return items[i].BlockNumber < items[j].BlockNumber
			})

			for _, receipt := range items {
				receipt := receipt
				if receipt.BlockNumber != 0 { // TODO: this is needed because we don't properly handle errors
					modelChan <- receipt
				}
			}
		}
	}

	extra := map[string]interface{}{
		"articulate": opts.Articulate,
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}
