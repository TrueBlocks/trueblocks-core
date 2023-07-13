package receiptsPkg

import (
	"context"
	"fmt"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *ReceiptsOptions) HandleShowReceipts() error {
	abiCache := articulate.NewAbiCache()
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode
	nErrors := 0

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawReceipt], errorChan chan error) {
		if txMap, err := identifiers.AsMap[types.SimpleReceipt](chain, opts.TransactionIds); err != nil {
			errorChan <- err
			cancel()
		} else {
			showProgress := !opts.Globals.TestMode && len(opts.Globals.File) == 0
			var bar = logger.NewBar("", showProgress, int64(len(txMap)))

			iterCtx, iterCancel := context.WithCancel(context.Background())
			defer iterCancel()

			iterFunc := func(app identifiers.ResolvedId, value *types.SimpleReceipt) error {
				if tx, err := app.FetchTransactionById(chain, false /* needsTraces */); err != nil {
					return fmt.Errorf("transaction at %s returned an error: %w", app.String(), err)
				} else if tx == nil || tx.Receipt == nil {
					return fmt.Errorf("transaction at %s has no logs", app.String())
				} else {
					if opts.Articulate {
						if err = abiCache.ArticulateReceipt(chain, tx.Receipt); err != nil {
							errorChan <- err // continue even with an error
						}
					}
					*value = *tx.Receipt
					bar.Tick()
					return nil
				}
			}

			iterErrorChan := make(chan error)
			go utils.IterateOverMap(iterCtx, iterErrorChan, txMap, iterFunc)
			for err := range iterErrorChan {
				// TODO: I don't really want to quit looping here. Just report the error and keep going.
				iterCancel()
				if !testMode || nErrors == 0 {
					errorChan <- err
					// Reporting more than one error causes tests to fail because they
					// appear concurrently so sort differently
					nErrors++
				}
			}
			bar.Finish(true)

			items := make([]types.SimpleReceipt, 0, len(txMap))
			for _, receipt := range txMap {
				items = append(items, *receipt)
			}
			sort.Slice(items, func(i, j int) bool {
				if items[i].BlockNumber == items[j].BlockNumber {
					return items[i].TransactionIndex < items[j].TransactionIndex
				}
				return items[i].BlockNumber < items[j].BlockNumber
			})

			for _, item := range items {
				item := item
				if item.BlockNumber != 0 {
					modelChan <- &item
				}
			}
		}
	}

	extra := map[string]interface{}{
		"articulate": opts.Articulate,
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}
