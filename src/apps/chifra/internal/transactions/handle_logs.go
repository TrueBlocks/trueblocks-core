// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package transactionsPkg

import (
	"context"
	"fmt"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *TransactionsOptions) HandleLogs(rCtx *output.RenderCtx) error {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode
	nErrors := 0

	abiCache := articulate.NewAbiCache(opts.Conn, opts.Articulate)
	logFilter := rpc.NewLogFilter(opts.Emitter, opts.Topic)

	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		apps, _, err := identifiers.IdsToApps(chain, opts.TransactionIds)
		if err != nil {
			errorChan <- err
			rCtx.Cancel()
		}

		if sliceOfMaps, cnt, err := types.AsSliceOfMaps[types.Transaction](apps, false); err != nil {
			errorChan <- err
			rCtx.Cancel()

		} else if cnt == 0 {
			errorChan <- fmt.Errorf("no transactions found")
			rCtx.Cancel()

		} else {
			showProgress := opts.Globals.ShowProgress()
			bar := logger.NewBar(logger.BarOptions{
				Enabled: showProgress,
				Total:   int64(cnt),
			})

			for _, thisMap := range sliceOfMaps {
				if rCtx.WasCanceled() {
					return
				}

				for app := range thisMap {
					thisMap[app] = new(types.Transaction)
				}

				iterFunc := func(app types.Appearance, value *types.Transaction) error {
					if tx, err := opts.Conn.GetTransactionByAppearance(&app, opts.Traces /* needsTraces */); err != nil {
						delete(thisMap, app)
						return fmt.Errorf("transaction at %s returned an error: %w", app.Orig(), err)

					} else if tx == nil {
						delete(thisMap, app)
						return fmt.Errorf("transaction at %s has no logs", app.Orig())

					} else {
						if opts.Articulate && tx.ArticulatedTx == nil {
							if err = abiCache.ArticulateTransaction(tx); err != nil {
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
				go utils.IterateOverMap(iterCtx, iterErrorChan, thisMap, iterFunc)
				for err := range iterErrorChan {
					if !testMode || nErrors == 0 {
						errorChan <- err
						nErrors++
					}
				}

				items := make([]types.Transaction, 0, len(thisMap))
				for _, tx := range thisMap {
					items = append(items, *tx)
				}
				sort.Slice(items, func(i, j int) bool {
					if items[i].BlockNumber == items[j].BlockNumber {
						return items[i].TransactionIndex < items[j].TransactionIndex
					}
					return items[i].BlockNumber < items[j].BlockNumber
				})

				for _, item := range items {
					for _, log := range item.Receipt.Logs {
						if logFilter.PassesFilter(&log) {
							modelChan <- &log
						}
					}
				}
			}
			bar.Finish(true /* newLine */)
		}
	}

	extraOpts := map[string]any{
		"logs":      opts.Logs,
		"traces":    opts.Traces,
		"addresses": opts.Uniq,
	}

	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOptsWithExtra(extraOpts))
}
