// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package exportPkg

import (
	"context"
	"fmt"
	"os"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/filter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ledger1"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ledger2"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ledger3"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ledger4"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *ExportOptions) HandleStatements(rCtx *output.RenderCtx, monitorArray []monitor.Monitor) error {
	testMode := opts.Globals.TestMode
	filter := filter.NewFilter(
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

	var recon ledger4.Reconcilerer
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

						apps := make([]types.Appearance, 0, len(thisMap))
						for _, tx := range txArray {
							apps = append(apps, types.Appearance{
								BlockNumber:      uint32(tx.BlockNumber),
								TransactionIndex: uint32(tx.TransactionIndex),
							})
						}

						ledgerOpts := &ledger4.ReconcilerOptions{
							Connection:   opts.Conn,
							AccountFor:   mon.Address,
							FirstBlock:   opts.FirstBlock,
							LastBlock:    opts.LastBlock,
							AsEther:      opts.Globals.Ether,
							TestMode:     testMode,
							UseTraces:    opts.Traces,
							Reversed:     opts.Reversed,
							AssetFilters: assetFilters,
						}
						if os.Getenv("NEW_CODE") == "3" {
							recon = ledger3.NewReconciler(ledgerOpts)
						} else if os.Getenv("NEW_CODE") == "true" {
							recon = ledger2.NewReconciler(ledgerOpts)
						} else {
							recon = ledger1.NewReconciler(ledgerOpts)
						}

						items := make([]types.Statement, 0, len(thisMap))
						for i, tx := range txArray {
							// Note: apps and txArray are the same list, so we can use the index from txArray
							prev := uint32(0)
							if apps[i].BlockNumber > 0 {
								prev = apps[i].BlockNumber - 1
							}
							if i > 0 {
								prev = apps[i-1].BlockNumber
							}
							next := apps[i].BlockNumber + 1
							if i < len(apps)-1 {
								next = apps[i+1].BlockNumber
							}
							pos := &types.AppPosition{
								Prev:  base.Blknum(prev),
								Next:  base.Blknum(next),
								First: i == 0,
								Last:  i == len(apps)-1,
							}
							if statements, err := recon.GetStatements(pos, filter, tx); err != nil {
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
	}

	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOptsWithExtra(extraOpts))
}
