// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package statePkg

import (
	"context"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/call"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/decache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

// func LocationsFromAddressEncodingAndBlockIds(conn *rpc.Connection, address base.Address, encoding string, ids []identifiers.Identifier) ([]cache.Locator, error) {
func (opts *StateOptions) HandleDecache() error {
	silent := opts.Globals.TestMode || len(opts.Globals.File) > 0

	var itemsToRemove []cache.Locator
	for _, addressStr := range opts.Addrs {
		address := base.HexToAddress(addressStr)
		if len(opts.Call) > 0 {
			callAddress := base.HexToAddress(opts.Addrs[0])
			if opts.ProxyFor != "" {
				callAddress = base.HexToAddress(opts.ProxyFor)
			}
			if contractCall, _, err := call.NewContractCall(opts.Conn, callAddress, opts.Call); err != nil {
				return err
			} else {
				if items, err := decache.LocationsFromAddressEncodingAndBlockIds(opts.Conn, address, contractCall.Method.Encoding, opts.BlockIds); err != nil {
					return err
				} else {
					itemsToRemove = append(itemsToRemove, items...)
				}
			}
			// } else {
		}
	}

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawModeler], errorChan chan error) {
		if msg, err := decache.Decache(opts.Conn, itemsToRemove, silent, walk.Cache_State); err != nil {
			errorChan <- err
		} else {
			s := types.SimpleMessage{
				Msg: msg,
			}
			modelChan <- &s
		}
	}

	opts.Globals.NoHeader = true
	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}
