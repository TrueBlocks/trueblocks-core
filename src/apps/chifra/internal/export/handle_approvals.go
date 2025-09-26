// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package exportPkg

import (
	"context"
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

func (opts *ExportOptions) HandleApprovals(rCtx *output.RenderCtx, monitorArray []monitor.Monitor) error {
	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		opts.Fourbytes = []string{"0x095ea7b3"}

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

		approvalTopic := base.HexToHash("0x8c5be1e5ebec7d5bd14f71427d1e84f3dd0314c0f7b2291e5b200ac8c7c3b925")
		logFilter := rpc.NewLogFilter([]string{}, []string{approvalTopic.Hex()})

		for _, mon := range monitorArray {
			if apps, cnt, err := mon.ReadAndFilterAppearances(filter, false /* withCount */); err != nil {
				errorChan <- err
				rCtx.Cancel()

			} else if cnt == 0 {
				continue

			} else {
				if sliceOfMaps, _, err := types.AsSliceOfMaps[types.Transaction](apps, filter.Reversed); err != nil {
					errorChan <- err
					rCtx.Cancel()

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

						// Instead of fetching full transactions, we'll fetch logs directly from blocks
						iterFunc := func(app types.Appearance, value *types.Transaction) error {
							// We still need some transaction data for the final output
							if tx, err := opts.Conn.GetTransactionByAppearance(&app, false); err != nil {
								return err
							} else {
								passes, _ := filter.ApplyTxFilters(tx)
								if passes {
									*value = *tx
								}
								if bar != nil {
									bar.Tick()
								}
								return nil
							}
						}

						// Set up and iterate over the map calling iterFunc for each appearance
						iterCtx, iterCancel := context.WithCancel(context.Background())
						defer iterCancel()
						errChan := make(chan error)
						go utils.IterateOverMap(iterCtx, errChan, thisMap, iterFunc)
						if stepErr := <-errChan; stepErr != nil {
							errorChan <- stepErr
							return
						}

						// Extract approval logs from the transactions
						items := make([]*types.Log, 0)
						for _, tx := range thisMap {
							if tx.Receipt == nil {
								continue
							}
							for _, log := range tx.Receipt.Logs {
								if filter.ApplyLogFilter(&log, addrArray) && logFilter.PassesFilter(&log) {
									if opts.Articulate {
										if err = abiCache.ArticulateLog(&log); err != nil {
											logger.Warn("Error articulating log:", err)
										}
									}
									items = append(items, &log)
								}
							}
						}

						// Sort the logs
						sort.Slice(items, func(i, j int) bool {
							if opts.Reversed {
								i, j = j, i
							}
							if items[i].BlockNumber == items[j].BlockNumber {
								if items[i].TransactionIndex == items[j].TransactionIndex {
									return items[i].LogIndex < items[j].LogIndex
								}
								return items[i].TransactionIndex < items[j].TransactionIndex
							}
							return items[i].BlockNumber < items[j].BlockNumber
						})

						// Send the logs to output
						for _, item := range items {
							var passes bool
							passes, finished = filter.ApplyCountFilter()
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
	}

	extraOpts := map[string]any{
		"articulate": opts.Articulate,
		"export":     true,
	}

	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOptsWithExtra(extraOpts))
}
