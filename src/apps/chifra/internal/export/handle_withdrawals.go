// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package exportPkg

import (
	"context"
	"fmt"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ranges"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *ExportOptions) HandleWithdrawals(rCtx *output.RenderCtx, monitorArray []monitor.Monitor) error {
	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		chain := opts.Globals.Chain
		testMode := opts.Globals.TestMode
		nErrors := 0
		first := max(base.KnownBlock(chain, "shanghai"), opts.FirstBlock)
		filter := types.NewFilter(
			opts.Reversed,
			false,
			[]string{},
			ranges.BlockRange{First: first, Last: opts.LastBlock},
			ranges.RecordRange{First: 0, Last: opts.GetMax()},
		)

		for _, mon := range monitorArray {
			if sliceOfMaps, cnt, err := monitor.AsSliceOfItemMaps[[]types.Withdrawal](&mon, filter, filter.Reversed); err != nil {
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
						thisMap[app] = &[]types.Withdrawal{}
					}

					iterFunc := func(app types.Appearance, value *[]types.Withdrawal) error {
						var block types.LightBlock
						if block, err = opts.Conn.GetBlockHeaderByNumber(base.Blknum(app.BlockNumber)); err != nil {
							return err
						}

						for _, w := range block.Withdrawals {
							if w.Address == mon.Address {
								*value = append(*value, w)
							}
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

					items := make([]*types.Withdrawal, 0, len(thisMap) * 2)
					for _, withdrawalSlice := range thisMap {
						if withdrawalSlice != nil && *withdrawalSlice != nil {
							for _, w := range *withdrawalSlice {
								items = append(items, &w)
							}
						}
					}

					sort.Slice(items, func(i, j int) bool {
						if opts.Reversed {
							i, j = j, i
						}
						if items[i].BlockNumber == items[j].BlockNumber {
							return items[i].Index < items[j].Index
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
		"export": true,
	}

	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOptsWithExtra(extraOpts))
}
