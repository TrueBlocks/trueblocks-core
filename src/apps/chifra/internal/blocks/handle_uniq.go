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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/uniq"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *BlocksOptions) HandleUniq(rCtx *output.RenderCtx) error {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode
	nErrors := 0

	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		apps, _, err := identifiers.IdsToApps(chain, opts.BlockIds)
		if err != nil {
			errorChan <- err
			rCtx.Cancel()
		}

		if sliceOfMaps, cnt, err := types.AsSliceOfMaps[types.Appearance](apps, false); err != nil {
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
				if rCtx.WasCanceled() {
					return
				}

				for app := range thisMap {
					thisMap[app] = new(types.Appearance)
				}

				apps := make([]types.Appearance, 0, len(thisMap))
				iterFunc := func(app types.Appearance, value *types.Appearance) error {
					_ = value
					bn := base.Blknum(app.BlockNumber)
					procFunc := func(s *types.Appearance) error {
						apps = append(apps, *s)
						return nil
					}

					if err := uniq.GetUniqAddressesInBlock(chain, opts.Flow, opts.Conn, procFunc, bn); err != nil {
						delete(thisMap, app)
						return err
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

				items := make([]types.Appearance, 0, len(thisMap))
				items = append(items, apps...)

				sort.Slice(items, func(i, j int) bool {
					if items[i].BlockNumber == items[j].BlockNumber {
						if items[i].TransactionIndex == items[j].TransactionIndex {
							return items[i].Reason < items[j].Reason
						}
						return items[i].TransactionIndex < items[j].TransactionIndex
					}
					return items[i].BlockNumber < items[j].BlockNumber
				})

				for _, item := range items {
					modelChan <- &item
				}
			}
			bar.Finish(true /* newLine */)
		}
	}

	extraOpts := map[string]any{
		"uniq": true,
	}

	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOptsWithExtra(extraOpts))
}
