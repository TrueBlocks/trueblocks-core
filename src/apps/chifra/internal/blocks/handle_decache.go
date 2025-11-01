// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package blocksPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/decache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/walk"
)

func (opts *BlocksOptions) HandleDecache(rCtx *output.RenderCtx) error {
	itemsToRemove, err := opts.getItemsToRemove()
	if err != nil {
		return err
	}

	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		showProgress := opts.Globals.ShowProgress()
		if msg, err := decache.Decache(opts.Conn, itemsToRemove, showProgress, opts.getCacheType()); err != nil {
			errorChan <- err
		} else {
			s := types.Message{
				Msg: msg,
			}
			modelChan <- &s
		}
	}

	opts.Globals.NoHeader = true
	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOpts())
}

func (opts *BlocksOptions) getCacheType() walk.CacheType {
	if opts.Logs {
		return walk.Cache_Logs
	} else if opts.Traces {
		return walk.Cache_Traces
	} else {
		return walk.Cache_Blocks
	}
}

func (opts *BlocksOptions) getItemsToRemove() ([]cache.Locator, error) {
	cT := opts.getCacheType()
	switch cT {
	case walk.Cache_Logs:
		itemsToRemove, err := decache.LocationsFromLogs(opts.Conn, opts.BlockIds)
		return itemsToRemove, err
	case walk.Cache_Traces:
		itemsToRemove, err := decache.LocationsFromTraces(opts.Conn, opts.BlockIds)
		return itemsToRemove, err
	case walk.Cache_Blocks:
		itemsToRemove, err := decache.LocationsFromBlocks(opts.Conn, opts.BlockIds)
		return itemsToRemove, err
	default:
		logger.ShouldNotHappen("Unknown cache type")
		return []cache.Locator{}, nil
	}
}
