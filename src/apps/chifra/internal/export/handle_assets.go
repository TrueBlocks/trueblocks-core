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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *ExportOptions) HandleAssets(rCtx *output.RenderCtx, monitorArray []monitor.Monitor) error {
	filter := types.NewFilter(
		opts.Reversed,
		opts.Reverted,
		opts.Fourbytes,
		base.BlockRange{First: opts.FirstBlock, Last: opts.LastBlock},
		base.RecordRange{First: opts.FirstRecord, Last: opts.GetMax()},
	)
	assetFilters := make([]base.Address, 0, len(opts.Asset))
	for _, asset := range opts.Asset {
		assetFilters = append(assetFilters, base.HexToAddress(asset))
	}

	assetMap := make(map[base.Address]types.Name)

	var recon *ledger.Reconciler
	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		for _, mon := range monitorArray {
			if apps, cnt, err := mon.ReadAndFilterAppearances(filter, false /* withCount */); err != nil {
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

					// TODO: BOGUS - THIS IS NOT CONCURRENCY SAFE
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

						// Set up and interate over the map calling iterFunc for each appearance
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

						recon = ledger.NewReconciler(opts.Conn, ledgerOpts)
						items := make([]types.Transfer, 0, len(thisMap))
						for _, tx := range txArray {
							if transfers, err := recon.GetTransfers(tx); err != nil {
								errorChan <- err

							} else if len(transfers) > 0 {
								items = append(items, transfers...)
							}
						}

						sort.Slice(items, func(i, j int) bool {
							if opts.Reversed {
								i, j = j, i
							}
							return items[i].Asset.LessThan(items[j].Asset)
						})

						for _, item := range items {
							var passes bool
							passes, finished = filter.ApplyCountFilter()
							if passes {
								if _, ok := assetMap[item.Asset]; !ok {
									// we've not seen this before -- report it
									if name, ok := recon.Names[item.Asset]; !ok {
										// TODO: we should use readContractAndClean (without updating
										// TODO: the name database here.
										// not found in global names map, build it
										name = types.Name{
											Address:  item.Asset,
											Name:     item.Asset.Display(3, 3),
											Decimals: 18,
										}
										modelChan <- &name
										assetMap[item.Asset] = name
									} else {
										// found in asset map, send it
										modelChan <- &name
										assetMap[item.Asset] = name
									}
								}
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
		"single": "asset",
	}

	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOptsWithExtra(extraOpts))
}
