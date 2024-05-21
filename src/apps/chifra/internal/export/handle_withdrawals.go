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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *ExportOptions) HandleWithdrawals(monitorArray []monitor.Monitor) error {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode
	nErrors := 0
	first := base.Max(base.KnownBlock(chain, "shanghai"), opts.FirstBlock)
	filter := filter.NewFilter(
		opts.Reversed,
		false,
		[]string{},
		base.BlockRange{First: first, Last: opts.LastBlock},
		// TODO: I feel (but have not investigated) that this may be a misake
		// TODO: Shouldn't the RecordRange start with zero not block number?
		// TODO: It means firstRecord, after all.
		base.RecordRange{First: uint64(first), Last: opts.GetMax()},
	)

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.Withdrawal], errorChan chan error) {
		for _, mon := range monitorArray {
			if apps, cnt, err := mon.ReadAndFilterAppearances(filter, false /* withCount */); err != nil {
				errorChan <- err
				cancel()

			} else if cnt == 0 {
				errorChan <- fmt.Errorf("no blocks found for the query")
				continue

			} else {
				if sliceOfMaps, _, err := types.AsSliceOfMaps[types.LightBlock](apps, filter.Reversed); err != nil {
					errorChan <- err
					cancel()

				} else {
					bar := logger.NewBar(logger.BarOptions{
						Prefix:  mon.Address.Hex(),
						Enabled: !testMode && !logger.IsTerminal(),
						Total:   int64(cnt),
					})

					// TODO: BOGUS - THIS IS NOT CONCURRENCY SAFE
					finished := false
					for _, thisMap := range sliceOfMaps {
						if finished {
							continue
						}

						for app := range thisMap {
							thisMap[app] = new(types.LightBlock)
						}

						iterFunc := func(app types.Appearance, value *types.LightBlock) error {
							var block types.LightBlock
							if block, err = opts.Conn.GetBlockHeaderByNumber2(base.Blknum(app.BlockNumber)); err != nil {
								return err
							}

							withdrawals := make([]types.Withdrawal, 0, 16)
							for _, w := range block.Withdrawals {
								if w.Address == mon.Address {
									withdrawals = append(withdrawals, w)
								}
							}
							if len(withdrawals) > 0 {
								block.Withdrawals = withdrawals
								*value = block
							}

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

						// Sort the items back into an ordered array by block number
						items := make([]*types.Withdrawal, 0, len(thisMap))
						for _, block := range thisMap {
							for _, with := range block.Withdrawals {
								items = append(items, &with)
							}
						}

						sort.Slice(items, func(i, j int) bool {
							if opts.Reversed {
								i, j = j, i
							}
							return items[i].BlockNumber < items[j].BlockNumber
						})

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

	extra := map[string]interface{}{
		"testMode": testMode,
		"export":   true,
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
