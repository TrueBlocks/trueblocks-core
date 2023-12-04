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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/filter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *ExportOptions) HandleLogs(monitorArray []monitor.Monitor) error {
	chain := opts.Globals.Chain
	abiCache := articulate.NewAbiCache(opts.Conn, opts.Articulate)
	testMode := opts.Globals.TestMode
	filter := filter.NewFilter(
		opts.Reversed,
		opts.Reverted,
		opts.Fourbytes,
		base.BlockRange{First: opts.FirstBlock, Last: opts.LastBlock},
		base.RecordRange{First: opts.FirstRecord, Last: opts.GetMax()},
	)

	addrArray := make([]base.Address, 0, len(monitorArray))
	for _, mon := range monitorArray {
		addrArray = append(addrArray, mon.Address)
	}

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawLog], errorChan chan error) {
		for _, mon := range monitorArray {
			if sliceOfMaps, cnt, err := monitor.AsSliceOfMaps[types.SimpleTransaction](&mon, 10, filter); err != nil {
				errorChan <- err
				cancel()

			} else if cnt == 0 {
				errorChan <- fmt.Errorf("no appearances found for %s", mon.Address.Hex())
				continue

			} else {
				bar := logger.NewBar(logger.BarOptions{
					Prefix:  mon.Address.Hex(),
					Enabled: !testMode && !utils.IsTerminal(),
					Total:   int64(cnt),
				})

				// TODO: BOGUS - THIS IS NOT CONCURRENCY SAFE
				finished := false
				for _, thisMap := range sliceOfMaps {
					if finished {
						continue
					}

					thisMap := thisMap
					for app := range thisMap {
						thisMap[app] = new(types.SimpleTransaction)
					}

					iterFunc := func(app types.SimpleAppearance, value *types.SimpleTransaction) error {
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

					// Set up and interate over the map calling iterFunc for each appearance
					iterCtx, iterCancel := context.WithCancel(context.Background())
					defer iterCancel()
					errChan := make(chan error)
					go utils.IterateOverMap(iterCtx, errChan, thisMap, iterFunc)
					if stepErr := <-errChan; stepErr != nil {
						errorChan <- stepErr
						return
					}

					items := make([]*types.SimpleLog, 0, len(thisMap))
					for _, tx := range thisMap {
						if tx.Receipt == nil {
							continue
						}
						for _, log := range tx.Receipt.Logs {
							log := log
							if filter.ApplyLogFilter(&log, addrArray) && opts.matchesFilter(&log) {
								if opts.Articulate {
									if err = abiCache.ArticulateLog(&log); err != nil {
										errorChan <- fmt.Errorf("error articulating log: %v", err)
									}
								}
								items = append(items, &log)
							}
						}
					}

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

					for _, item := range items {
						item := item
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

	extra := map[string]interface{}{
		"articulate": opts.Articulate,
		"testMode":   testMode,
		"export":     true,
	}

	if opts.Globals.Verbose || opts.Globals.Format == "json" {
		parts := names.Custom | names.Prefund | names.Regular
		if namesMap, err := names.LoadNamesMap(chain, parts, nil); err != nil {
			return err
		} else {
			extra["namesMap"] = namesMap
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}
