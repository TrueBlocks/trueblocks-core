// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package blocksPkg

import (
	"context"
	"fmt"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *BlocksOptions) HandleWithdrawals(rCtx *output.RenderCtx) error {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode
	nErrors := 0

	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		apps, _, err := identifiers.IdsToApps(chain, opts.BlockIds)
		if err != nil {
			errorChan <- err
			rCtx.Cancel()
		}

		if sliceOfMaps, cnt, err := types.AsSliceOfMaps[types.LightBlock](apps, false); err != nil {
			errorChan <- err
			rCtx.Cancel()

		} else if cnt == 0 {
			errorChan <- fmt.Errorf("no blocks found for the query")
			rCtx.Cancel()

		} else {
			showProgress := opts.Globals.ShowProgress()
			bar := logger.NewBar(logger.BarOptions{
				Enabled: showProgress,
				Total:   int64(cnt),
			})

			for _, thisMap := range sliceOfMaps {
				for app := range thisMap {
					thisMap[app] = new(types.LightBlock)
				}

				items := make([]*types.Withdrawal, 0, len(thisMap))
				iterFunc := func(app types.Appearance, value *types.LightBlock) error {
					bn := base.Blknum(app.BlockNumber)
					if block, err := opts.Conn.GetBlockHeaderByNumber(bn); err != nil {
						delete(thisMap, app)
						return err
					} else {
						*value = block
						bar.Tick()
					}
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

				for _, item := range thisMap {
					for _, w := range item.Withdrawals {
						w.BlockNumber = item.BlockNumber
						items = append(items, &w)
					}
				}

				sort.Slice(items, func(i, j int) bool {
					if items[i].BlockNumber == items[j].BlockNumber {
						return items[i].Index < items[j].Index
					}
					return items[i].BlockNumber < items[j].BlockNumber
				})

				for _, item := range items {
					modelChan <- item
				}
			}
			bar.Finish(true /* newLine */)
		}
	}

	return output.StreamMany(rCtx.Ctx, fetchData, opts.Globals.OutputOpts())
}
