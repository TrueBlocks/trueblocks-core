// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package blocksPkg

import (
	"context"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/decache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

func (opts *BlocksOptions) HandleDecache() error {
	silent := opts.Globals.TestMode || len(opts.Globals.File) > 0

	itemsToRemove, err := decache.LocationsFromBlockIds(opts.Conn, opts.BlockIds, opts.Logs, opts.Traces)
	if err != nil {
		return err
	}

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawModeler], errorChan chan error) {
		if msg, err := decache.Decache(opts.Conn, itemsToRemove, silent, opts.getCacheType()); err != nil {
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

func (opts *BlocksOptions) getCacheType() walk.CacheType {
	cT := walk.Cache_Blocks
	if opts.Logs {
		cT = walk.Cache_Logs
	} else if opts.Traces {
		cT = walk.Cache_Traces
	}
	return cT
}
