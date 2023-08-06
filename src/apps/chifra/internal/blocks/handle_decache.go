// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package blocksPkg

import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache/locations"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *BlocksOptions) HandleDecache() error {
	chain := opts.Globals.Chain
	opts.Conn = rpcClient.NewReadOnlyConnection(chain)

	toRemove, err := opts.LocationsFromIdentifiers()
	if err != nil {
		return err
	}

	testMode := opts.Globals.TestMode
	itemsSeen := int64(0)
	itemsProcessed := int64(0)
	bytesProcessed := 0
	processorFunc := func(info *locations.ItemInfo) bool {
		itemsSeen++
		itemsProcessed++
		bytesProcessed += info.Size()
		logger.Progress(!testMode && itemsProcessed%20 == 0, "Removed", itemsProcessed, "items and", bytesProcessed, "bytes.", info.Name())
		if opts.Globals.Verbose {
			logger.Info(info.Name(), "was removed.")
		}

		return true
	}

	_ = opts.Conn.Store.Decache(toRemove, processorFunc)
	logger.Info(itemsProcessed, "items totaling", bytesProcessed, "bytes were removed from the cache.", strings.Repeat(" ", 60))

	return nil
}

// TODO: We could use a Modeler that only delivers a message (i.e. SimpleModeler). Use it here and
// TODO: in monitors --decache to report some data in case the standard error is redirected.

func (opts *BlocksOptions) LocationsFromIdentifiers() ([]cache.Locator, error) {
	chain := opts.Globals.Chain
	toRemove := make([]cache.Locator, 0)
	for _, br := range opts.BlockIds {
		blockNums, err := br.ResolveBlocks(chain)
		if err != nil {
			return nil, err
		}
		for _, bn := range blockNums {
			rawBlock, err := opts.Conn.GetBlockHeaderByNumber(bn)
			if err != nil {
				return nil, err
			}
			toRemove = append(toRemove, &types.SimpleBlock[string]{
				BlockNumber: bn,
			})
			for index := range rawBlock.Transactions {
				txToRemove := &types.SimpleTransaction{
					BlockNumber:      bn,
					TransactionIndex: uint64(index),
				}
				toRemove = append(toRemove, txToRemove)
				toRemove = append(toRemove, &types.SimpleTraceGroup{
					BlockNumber:      bn,
					TransactionIndex: base.Txnum(index),
				})
			}
		}
	}
	return toRemove, nil
}
