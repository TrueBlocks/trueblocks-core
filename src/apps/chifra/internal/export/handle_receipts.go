// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package exportPkg

import (
	"context"
	"fmt"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ranges"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *ExportOptions) HandleReceipts(rCtx *output.RenderCtx, monitorArray []monitor.Monitor) error {
	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		abiCache := articulate.NewAbiCache(opts.Conn, opts.Articulate)
		filter := types.NewFilter(
			opts.Reversed,
			opts.Reverted,
			opts.Fourbytes,
			ranges.BlockRange{First: opts.FirstBlock, Last: opts.LastBlock},
			ranges.RecordRange{First: opts.FirstRecord, Last: opts.GetMax()},
		)

		addrArray := make([]base.Address, 0, len(monitorArray))
		for _, mon := range monitorArray {
			addrArray = append(addrArray, mon.Address)
		}
		logFilter := rpc.NewLogFilter(opts.Emitter, opts.Topic)

		for _, mon := range monitorArray {
			if sliceOfMaps, cnt, err := monitor.AsSliceOfItemMaps[types.Transaction](&mon, filter, filter.Reversed); err != nil {
				errorChan <- err
				rCtx.Cancel()

			} else if cnt == 0 {
				errorChan <- fmt.Errorf("no blocks found for the query")
				continue

			} else {
				showProgress := opts.Globals.ShowProgress()
				bar := logger.NewBar(logger.BarOptions{
					Prefix:  mon.Address.Hex(),
					Enabled: showProgress,
					Total:   int64(cnt),
				})

				finished := false
				for _, thisMap := range sliceOfMaps {
					if rCtx.WasCanceled() {
						return
					}

					if finished {
						continue
					}

					for app := range thisMap {
						thisMap[app] = new(types.Transaction)
					}

					iterFunc := func(app types.Appearance, value *types.Transaction) error {
						if tx, err := opts.Conn.GetTransactionByAppearance(&app, false); err != nil {
							return err
						} else {
							passes := filter.PassesTxFilter(tx)
							if passes {
								*value = *tx
							}
							if bar != nil {
								bar.Tick()
							}
							return nil
						}
					}

					// Set up and interate over the map calling iterFunc for each appearance
					iterCtx, iterCancel := context.WithCancel(context.Background())
					defer iterCancel()
					errChan := make(chan error)
					go utils.IterateOverMap(iterCtx, errChan, thisMap, iterFunc)
					if stepErr := <-errChan; stepErr != nil {
						errorChan <- stepErr
						return
					}

					items := make([]*types.Receipt, 0, len(thisMap))
					for _, tx := range thisMap {
						if tx.Receipt == nil {
							continue
						}
						filteredLogs := make([]types.Log, 0, len(tx.Receipt.Logs))
						for _, log := range tx.Receipt.Logs {
							if filter.PassesLogFilter(&log, addrArray) && logFilter.PassesFilter(&log) {
								if opts.Articulate {
									if err := abiCache.ArticulateLog(&log); err != nil {
										errorChan <- fmt.Errorf("error articulating log: %v", err)
									}
								}
								filteredLogs = append(filteredLogs, log)
							}
						}
						tx.Receipt.Logs = filteredLogs
						items = append(items, tx.Receipt)
					}

					sort.Slice(items, func(i, j int) bool {
						if opts.Reversed {
							i, j = j, i
						}
						if items[i].BlockNumber == items[j].BlockNumber {
							return items[i].TransactionIndex < items[j].TransactionIndex
						}
						return items[i].BlockNumber < items[j].BlockNumber
					})

					for _, item := range items {
						var passes bool
						passes, finished = filter.PassesCountFilter()
						if passes {
							modelChan <- item
						}
						if finished {
							break
						}
					}
				}
				bar.Finish(true /* newLine */)
			}
		}
	}

	extraOpts := map[string]any{
		"articulate": opts.Articulate,
		"export":     true,
	}

	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOptsWithExtra(extraOpts))
}
