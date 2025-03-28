// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package exportPkg

import (
	"context"
	"fmt"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ledger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ranges"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *ExportOptions) HandleStatements(rCtx *output.RenderCtx, monitorArray []monitor.Monitor) error {
	filter := types.NewFilter(
		opts.Reversed,
		opts.Reverted,
		opts.Fourbytes,
		ranges.BlockRange{First: opts.FirstBlock, Last: opts.LastBlock},
		ranges.RecordRange{First: opts.FirstRecord, Last: opts.GetMax()},
	)
	assetFilters := make([]base.Address, 0, len(opts.Asset))
	for _, asset := range opts.Asset {
		assetFilters = append(assetFilters, base.HexToAddress(asset))
	}

	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		for _, mon := range monitorArray {
			ledgerOpts := &ledger.ReconcilerOptions{
				AccountFor:   mon.Address,
				FirstBlock:   opts.FirstBlock,
				LastBlock:    opts.LastBlock,
				AsEther:      opts.Globals.Ether,
				UseTraces:    opts.Traces,
				Reversed:     opts.Reversed,
				AssetFilters: assetFilters,
				AppFilters:   filter,
			}
			recon := ledger.NewReconciler(opts.Conn, ledgerOpts)

			if apps, cnt, err := mon.ReadAndFilterAppearances(filter, false); err != nil {
				errorChan <- err
				rCtx.Cancel()
			} else if cnt == 0 {
				errorChan <- fmt.Errorf("no blocks found for the query")
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

						iterFunc := func(app types.Appearance, value *types.Transaction) error {
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

						iterCtx, iterCancel := context.WithCancel(context.Background())
						defer iterCancel()
						errChan := make(chan error)
						go utils.IterateOverMap(iterCtx, errChan, thisMap, iterFunc)
						if stepErr := <-errChan; stepErr != nil {
							errorChan <- stepErr
							return
						}

						txArray := make([]*types.Transaction, 0, len(thisMap))
						for _, tx := range thisMap {
							txArray = append(txArray, tx)
						}

						sort.Slice(txArray, func(i, j int) bool {
							if txArray[i].BlockNumber == txArray[j].BlockNumber {
								return txArray[i].TransactionIndex < txArray[j].TransactionIndex
							}
							return txArray[i].BlockNumber < txArray[j].BlockNumber
						})

						apps := make([]types.Appearance, 0, len(thisMap))
						for _, tx := range txArray {
							app := types.Appearance{
								BlockNumber:      uint32(tx.BlockNumber),
								TransactionIndex: uint32(tx.TransactionIndex),
							}
							logger.TestLog(true, fmt.Sprintf("%d:", len(apps)), app.BlockNumber, app.TransactionIndex)
							apps = append(apps, app)
						}

						items := make([]types.Statement, 0, len(thisMap))
						list, err := types.NewAppListFromApps(apps, txArray)
						if err != nil {
							errorChan <- err
						}
						i := -1
						for node := list.Head; node != nil; node = node.Next() {
							i++
							if statements, err := recon.GetStatements(node); err != nil {
								errorChan <- err

							} else if len(statements) > 0 {
								items = append(items, statements...)
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
							var passes bool
							passes, finished = filter.ApplyCountFilter()
							if passes {
								if opts.Globals.Ether {
									item.Symbol = "ETH"
								}
								modelChan <- &item
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
		"loadNames":  true,
	}

	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOptsWithExtra(extraOpts))
}
