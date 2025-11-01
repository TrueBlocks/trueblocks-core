// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package exportPkg

import (
	"context"
	"fmt"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/ranges"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/topics"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/utils"
)

func (opts *ExportOptions) HandleApprovals(rCtx *output.RenderCtx, monitorArray []monitor.Monitor) error {
	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		abiCache := articulate.NewAbiCache(opts.Conn, opts.Articulate)
		filter := types.NewFilter(
			opts.Reversed,
			false,
			[]string{topics.ApprovalFourbyte},
			ranges.BlockRange{First: opts.FirstBlock, Last: opts.LastBlock},
			ranges.RecordRange{First: opts.FirstRecord, Last: opts.GetMax()},
		)

		addrArray := make([]base.Address, 0, len(monitorArray))
		for _, mon := range monitorArray {
			addrArray = append(addrArray, mon.Address)
		}
		logFilter := rpc.NewLogFilter(opts.Emitter, []string{topics.ApprovalTopic.Hex()})

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
							passesFourByte := filter.PassesTxFilter(tx)
							hasApprovalLogs := false
							if tx.Receipt != nil {
								filteredLogs := make([]types.Log, 0, len(tx.Receipt.Logs))
								hasEmitterLogs := len(opts.Emitter) == 0
								for _, log := range tx.Receipt.Logs {
									if !hasEmitterLogs {
										for _, emitter := range opts.Emitter {
											if log.Address.Hex() == emitter {
												hasEmitterLogs = true
												break
											}
										}
									}

									if filter.PassesLogFilter(&log, addrArray) && logFilter.PassesFilter(&log) {
										log.BlockHash = tx.BlockHash
										log.BlockNumber = tx.BlockNumber
										log.TransactionHash = tx.Hash
										log.TransactionIndex = tx.TransactionIndex
										filteredLogs = append(filteredLogs, log)
										hasApprovalLogs = true
									}
								}

								shouldInclude := (passesFourByte && hasEmitterLogs) || hasApprovalLogs
								if shouldInclude {
									tx.Receipt.Logs = filteredLogs
									*value = *tx
								}
							} else if passesFourByte && len(opts.Emitter) == 0 {
								*value = *tx
							}
							if bar != nil {
								bar.Tick()
							}
							return nil
						}
					}

					// Set up and iterate over the map calling iterFunc for each appearance (PARALLEL)
					iterCtx, iterCancel := context.WithCancel(context.Background())
					defer iterCancel()
					errChan := make(chan error)
					go utils.IterateOverMap(iterCtx, errChan, thisMap, iterFunc)
					if stepErr := <-errChan; stepErr != nil {
						errorChan <- stepErr
						return
					}

					items := make([]*types.Transaction, 0, len(thisMap))
					for _, tx := range thisMap {
						if !tx.BlockHash.IsZero() {
							if opts.Articulate {
								if err := abiCache.ArticulateTransaction(tx); err != nil {
									errorChan <- err // continue even on error
								}
							}
							items = append(items, tx)
						}
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
						if item.BlockHash.IsZero() {
							continue
						}
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
