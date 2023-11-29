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

func (opts *ReceiptsOptions) HandleShow() error {
	chain := opts.Globals.Chain
	abiCache := articulate.NewAbiCache(opts.Conn, opts.Articulate)
	testMode := opts.Globals.TestMode
	nErrors := 0

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawReceipt], errorChan chan error) {
		var err error
		var appMap map[types.SimpleAppearance]*types.SimpleTransaction
		if appMap, _, err = identifiers.AsMap[types.SimpleTransaction](chain, opts.TransactionIds); err != nil {
			errorChan <- err
			cancel()
		} else {
			bar := logger.NewBar(logger.BarOptions{
				Enabled: !opts.Globals.TestMode,
				Total:   int64(len(appMap)),
			})

			iterCtx, iterCancel := context.WithCancel(context.Background())
			defer iterCancel()

			iterFunc := func(app types.SimpleAppearance, value *types.SimpleTransaction) error {
				a := &types.RawAppearance{
					BlockNumber:      uint32(app.BlockNumber),
					TransactionIndex: uint32(app.TransactionIndex),
				}
				if tx, err := opts.Conn.GetTransactionByAppearance(a, false /* needsTraces */); err != nil {
					return fmt.Errorf("transaction at %s returned an error: %w", app.Orig(), err)
				} else if tx == nil || tx.Receipt == nil {
					return fmt.Errorf("transaction at %s has no logs", app.Orig())
				} else {
					if opts.Articulate {
						if err = abiCache.ArticulateReceipt(tx.Receipt); err != nil {
							errorChan <- err // continue even with an error
						}
					}
					*value = *tx
					bar.Tick()
					return nil
				}
			}

			iterErrorChan := make(chan error)
			go utils.IterateOverMap(iterCtx, iterErrorChan, appMap, iterFunc)
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

			items := make([]types.SimpleReceipt, 0, len(appMap))
			for _, tx := range appMap {
				if tx.Receipt != nil {
					items = append(items, *tx.Receipt)
				}
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
