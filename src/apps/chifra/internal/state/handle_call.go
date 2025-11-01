// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package statePkg

import (
	"context"
	"fmt"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/call"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/utils"
)

func (opts *StateOptions) HandleCall(rCtx *output.RenderCtx) error {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode
	nErrors := 0

	artFunc := func(str string, function *types.Function) error {
		return articulate.ArticulateFunction(function, "", str[2:])
	}

	callAddress := opts.GetAddressOrProxy()
	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		apps, _, err := identifiers.IdsToApps(chain, opts.BlockIds)
		if err != nil {
			errorChan <- err
			rCtx.Cancel()
		}

		if sliceOfMaps, cnt, err := types.AsSliceOfMaps[[]types.Result](apps, false); err != nil {
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
					thisMap[app] = new([]types.Result)
				}

				iterFunc := func(app types.Appearance, value *[]types.Result) error {
					bn := base.Blknum(app.BlockNumber)
					for _, c := range opts.Calls {
						if contractCall, _, err := call.NewContractCall(opts.Conn, callAddress, c); err != nil {
							delete(thisMap, app)
							return fmt.Errorf("the --calldata value provided (%s) was not found: %s", c, err)

						} else {
							contractCall.BlockNumber = bn
							results, err := contractCall.Call(artFunc)
							if err != nil {
								delete(thisMap, app)
								return err
							} else {
								bar.Tick()
								*value = append(*value, *results)
							}
						}
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

				items := make([]types.Result, 0, len(thisMap))
				for _, v := range thisMap {
					items = append(items, *v...)
				}

				sort.Slice(items, func(i, j int) bool {
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
		"articulate": opts.Articulate,
	}

	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOptsWithExtra(extraOpts))
}

func (opts *StateOptions) GetAddressOrProxy() base.Address {
	// Note that validtor makes sure there's exactly one address for --call or --send.
	callAddress := base.HexToAddress(opts.Addrs[0])
	proxy := opts.ProxyForAddr
	if !proxy.IsZero() {
		callAddress = proxy
	}
	return callAddress
}
