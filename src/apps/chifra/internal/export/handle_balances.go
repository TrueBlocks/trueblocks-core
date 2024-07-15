// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package exportPkg

import (
	"context"
	"fmt"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/filter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *ExportOptions) HandleBalances(rCtx *output.RenderCtx, monitorArray []monitor.Monitor) error {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode
	nErrors := 0

	filter := filter.NewFilter(
		opts.Reversed,
		opts.Reverted,
		opts.Fourbytes,
		base.BlockRange{First: opts.FirstBlock, Last: opts.LastBlock},
		base.RecordRange{First: opts.FirstRecord, Last: opts.GetMax()},
	)

	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		currentBn := base.Blknum(0)
		prevBalance := base.NewWei(0)

		for _, mon := range monitorArray {
			if apps, cnt, err := mon.ReadAndFilterAppearances(filter, false /* withCount */); err != nil {
				errorChan <- err
				rCtx.Cancel()

			} else if cnt == 0 {
				errorChan <- fmt.Errorf("no blocks found for the query")
				continue

			} else {
				if sliceOfMaps, _, err := types.AsSliceOfMaps[types.Token](apps, filter.Reversed); err != nil {
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
					prevBalance, _ = opts.Conn.GetBalanceAt(mon.Address, filter.GetOuterBounds().First)
					for _, thisMap := range sliceOfMaps {
						if rCtx.WasCanceled() {
							return
						}

						if finished {
							continue
						}

						for app := range thisMap {
							thisMap[app] = new(types.Token)
						}

						iterFunc := func(app types.Appearance, value *types.Token) error {
							var balance *base.Wei
							if balance, err = opts.Conn.GetBalanceAt(mon.Address, base.Blknum(app.BlockNumber)); err != nil {
								return err
							}
							value.Address = base.FAKE_ETH_ADDRESS
							value.Holder = mon.Address
							value.BlockNumber = base.Blknum(app.BlockNumber)
							value.TransactionIndex = base.Txnum(app.TransactionIndex)
							value.Balance = *balance
							value.Timestamp = app.Timestamp
							bar.Tick()
							return nil
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

						items := make([]*types.Token, 0, len(thisMap))
						for _, tx := range thisMap {
							items = append(items, tx)
						}

						sort.Slice(items, func(i, j int) bool {
							if opts.Reversed {
								i, j = j, i
							}
							return items[i].BlockNumber < items[j].BlockNumber
						})

						for idx, item := range items {
							visitToken := func(idx int, item *types.Token) error {
								item.PriorBalance = *prevBalance
								if item.BlockNumber == 0 || item.BlockNumber != currentBn || item.Timestamp == 0xdeadbeef {
									item.Timestamp, _ = tslib.FromBnToTs(chain, item.BlockNumber)
								}
								currentBn = item.BlockNumber
								if idx == 0 || item.PriorBalance.Cmp(&item.Balance) != 0 || opts.Globals.Verbose {
									var passes bool
									passes, finished = filter.ApplyCountFilter()
									if passes {
										modelChan <- item
									}
								}
								prevBalance = &item.Balance
								return nil
							}
							if err := visitToken(idx, item); err != nil {
								errorChan <- err
								return
							}
							if finished {
								break
							}
						}
					}
					bar.Finish(true /* newLine */)
				}
				prevBalance = base.NewWei(0)
			}

		}
	}

	extraOpts := map[string]any{
		"export": true,
		"parts":  []string{"blockNumber", "date", "holder", "balance", "diff", "balanceDec"},
	}

	return output.StreamMany(rCtx.Ctx, fetchData, opts.Globals.OutputOptsWithExtra(extraOpts))
}
