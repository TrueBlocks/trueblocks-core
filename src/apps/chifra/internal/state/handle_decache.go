// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package statePkg

import (
	"context"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/call"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/decache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

func (opts *StateOptions) HandleDecache() error {
	var itemsToRemove []cache.Locator
	for _, addressStr := range opts.Addrs {
		address := base.HexToAddress(addressStr)
		if len(opts.Call) > 0 {
			callAddress := base.HexToAddress(opts.Addrs[0])
			proxy := base.HexToAddress(opts.ProxyFor)
			if !proxy.IsZero() {
				callAddress = proxy
			}
			for _, c := range opts.Call {
				if contractCall, _, err := call.NewContractCall(opts.Conn, callAddress, c); err != nil {
					wrapped := fmt.Errorf("the --call value provided (%s) was not found: %s", c, err)
					return wrapped
				} else {
					if items, err := decache.LocationsFromAddressEncodingAndBlockIds(opts.Conn, address, contractCall.Method.Encoding, opts.BlockIds); err != nil {
						return err
					} else {
						itemsToRemove = append(itemsToRemove, items...)
					}
				}
			}
		}
	}

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		showProgress := opts.Globals.ShowProgress()
		if msg, err := decache.Decache(opts.Conn, itemsToRemove, showProgress, walk.Cache_State); err != nil {
			errorChan <- err
		} else {
			s := types.Message{
				Msg: msg,
			}
			modelChan <- &s
		}
	}

	opts.Globals.NoHeader = true
	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}
