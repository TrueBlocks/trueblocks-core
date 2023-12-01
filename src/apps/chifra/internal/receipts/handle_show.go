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
		// var cnt int
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

			iterFunc := func(app types.SimpleAppearance, value *types.SimpleTransaction) error {
				if tx, err := opts.Conn.GetTransactionByAppearance(&app, false /* needsTraces */); err != nil {
					return fmt.Errorf("transaction at %s returned an error: %w", app.Orig(), err)
				} else if tx == nil || tx.Receipt == nil {
					return fmt.Errorf("transaction at %s has no receipts", app.Orig())
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
			iterCtx, iterCancel := context.WithCancel(context.Background())
			defer iterCancel()
			go utils.IterateOverMap(iterCtx, iterErrorChan, appMap, iterFunc)
			for err := range iterErrorChan {
				if !testMode || nErrors == 0 {
					errorChan <- err
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
