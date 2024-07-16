// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package statePkg

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/call"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/decache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

func (opts *StateOptions) HandleDecache(rCtx *output.RenderCtx) error {
	itemsToRemove, err := opts.getItemsToRemove()
	if err != nil {
		return err
	}

	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		showProgress := opts.Globals.ShowProgress()
		monitorCacheTypes := []walk.CacheType{
			walk.Cache_State,
			walk.Cache_Results,
		}
		for _, cacheType := range monitorCacheTypes {
			if msg, err := decache.Decache(opts.Conn, itemsToRemove, showProgress, cacheType); err != nil {
				errorChan <- err
			} else {
				s := types.Message{
					Msg: msg,
				}
				modelChan <- &s
			}
		}
	}

	opts.Globals.NoHeader = true
	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOpts())
}

func (opts *StateOptions) getItemsToRemove() ([]cache.Locator, error) {
	allItems := make([]cache.Locator, 0)
	for _, addr := range opts.Addrs {
		address := base.HexToAddress(addr)
		itemsToRemove, err := decache.LocationsFromState(opts.Conn, address, opts.BlockIds)
		if err != nil {
			return []cache.Locator{}, err
		}
		allItems = append(allItems, itemsToRemove...)

		for _, c := range opts.Calls {
			if len(c) > 0 {
				callAddress := opts.GetCallAddress()
				if contractCall, _, err := call.NewContractCall(opts.Conn, callAddress, c); err != nil {
					wrapped := fmt.Errorf("the --call value provided (%s) was not found: %s", c, err)
					return []cache.Locator{}, wrapped

				} else {
					itemsToRemove, err := decache.LocationsFromAddressAndEncodings(opts.Conn, address, contractCall.Method.Encoding, opts.BlockIds)
					if err != nil {
						return []cache.Locator{}, err
					}
					allItems = append(allItems, itemsToRemove...)
				}
			}
		}
	}
	return allItems, nil
}
