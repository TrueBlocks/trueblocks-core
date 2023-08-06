// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package blocksPkg

import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache/locations"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *BlocksOptions) HandleDecache() error {
	chain := opts.Globals.Chain
	opts.Conn = rpcClient.NewReadOnlyConnection(chain)

	toRemove := make([]cache.Locator, 0)
	for _, br := range opts.BlockIds {
		blockNums, err := br.ResolveBlocks(chain)
		if err != nil {
			return err
		}
		for _, bn := range blockNums {
			rawBlock, err := opts.Conn.GetBlockBodyByNumber(bn)
			if err != nil {
				return err
			}
			toRemove = append(toRemove, &types.SimpleBlock[string]{
				BlockNumber: bn,
			})
			for _, tx := range rawBlock.Transactions {
				txToRemove := &types.SimpleTransaction{
					BlockNumber:      bn,
					TransactionIndex: tx.TransactionIndex,
				}
				toRemove = append(toRemove, txToRemove)
				toRemove = append(toRemove, &types.SimpleTraceGroup{
					BlockNumber:      tx.BlockNumber,
					TransactionIndex: int(tx.TransactionIndex),
				})
			}
		}
	}

	testMode := opts.Globals.TestMode
	itemsSeen := int64(0)
	itemsRemoved := int64(0)
	bytesRemoved := 0
	processorFunc := func(info *locations.ItemInfo) bool {
		itemsSeen++
		itemsRemoved++
		bytesRemoved += info.Size()
		logger.Progress(!testMode && itemsRemoved%20 == 0, "Removed", itemsRemoved, "items and", bytesRemoved, "bytes.", info.Name())
		if opts.Globals.Verbose {
			logger.Info(info.Name(), "was removed.")
		}

		return true
	}

	_ = opts.Conn.Store.Decache(toRemove, processorFunc)

	if itemsSeen == 0 {
		logger.Info("No items matching the query were found in the cache.", strings.Repeat(" ", 60))
	} else {
		logger.Info(itemsRemoved, "items totaling", bytesRemoved, "bytes were removed from the cache.", strings.Repeat(" ", 60))
	}

	return nil
}

// TODO: We could use a Modeler that only delivers a message (i.e. SimpleModeler). Use it here and
// TODO: in monitors --decache to report some data in case the standard error is redirected.
