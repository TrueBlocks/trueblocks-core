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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *ExportOptions) HandleBalances(monitorArray []monitor.Monitor) error {
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

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawToken], errorChan chan error) {
		currentBn := uint64(0)
		prevBalance := base.NewMyWei(0)

		for _, mon := range monitorArray {
			if apps, cnt, err := mon.ReadAndFilterAppearances(filter, false /* withCount */); err != nil {
				errorChan <- err
				cancel()

			} else if cnt == 0 {
				errorChan <- fmt.Errorf("no blocks found for the query")
				continue

			} else {
				if sliceOfMaps, _, err := types.AsSliceOfMaps[types.SimpleToken](apps, filter.Reversed); err != nil {
					errorChan <- err
					cancel()

				} else {
					bar := logger.NewBar(logger.BarOptions{
						Prefix:  mon.Address.Hex(),
						Enabled: !testMode && !utils.IsTerminal(),
						Total:   int64(cnt),
					})

					// TODO: BOGUS - THIS IS NOT CONCURRENCY SAFE
					finished := false
					prevBalance, _ = opts.Conn.GetBalanceAt(mon.Address, filter.GetOuterBounds().First)
					for _, thisMap := range sliceOfMaps {
						if finished {
							continue
						}

						for app := range thisMap {
							thisMap[app] = new(types.SimpleToken)
						}

						iterFunc := func(app types.SimpleAppearance, value *types.SimpleToken) error {
							var balance *base.MyWei
							if balance, err = opts.Conn.GetBalanceAt(mon.Address, uint64(app.BlockNumber)); err != nil {
								return err
							}
							value.Address = base.FAKE_ETH_ADDRESS
							value.Holder = mon.Address
							value.BlockNumber = uint64(app.BlockNumber)
							value.TransactionIndex = uint64(app.TransactionIndex)
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

						items := make([]*types.SimpleToken, 0, len(thisMap))
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
							visitToken := func(idx int, item *types.SimpleToken) error {
								item.PriorBalance = *prevBalance
								if item.BlockNumber == 0 || item.BlockNumber != currentBn || item.Timestamp == 0xdeadbeef {
									item.Timestamp, _ = tslib.FromBnToTs(chain, item.BlockNumber)
								}
								currentBn = item.BlockNumber
								if idx == 0 || item.PriorBalance.Cmp(&item.Balance) != 0 || opts.Globals.Verbose {
									item.Diff = *base.NewMyWei(0).Sub(&item.Balance, &item.PriorBalance)
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
				prevBalance = base.NewMyWei(0)
			}

		}
	}

	extra := map[string]interface{}{
		"testMode": testMode,
		"export":   true,
		"parts":    []string{"blockNumber", "date", "holder", "balance", "diff", "units"},
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
