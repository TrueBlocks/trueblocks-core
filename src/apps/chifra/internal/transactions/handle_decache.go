// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package transactionsPkg

import (
	"errors"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cacheNew"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cacheNew/locations"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum"
)

func (opts *TransactionsOptions) HandleDecache() error {
	cache := opts.Globals.CacheStore(true)
	toRemove := make([]cacheNew.Locator, 0)
	for _, rng := range opts.TransactionIds {
		txIds, err := rng.ResolveTxs(opts.Globals.Chain)
		if err != nil && !errors.Is(err, ethereum.NotFound) {
			continue
		}
		for _, app := range txIds {
			toRemove = append(toRemove, &types.SimpleTransaction{
				BlockNumber:      uint64(app.BlockNumber),
				TransactionIndex: uint64(app.TransactionIndex),
			})
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
		logger.Info(!testMode && itemsRemoved%20 == 0, "Removed ", itemsRemoved, " items and ", bytesRemoved, " bytes.", info.Name())

		if opts.Globals.Verbose {
			logger.Info(info.Name(), "was removed.")
		}

		return true
	}

	if err := cache.Decache(toRemove, processorFunc); err != nil {
		return err
	}

	if itemsSeen == 0 {
		logger.Info("No items matching the query were found in the cache.", strings.Repeat(" ", 60))
	} else {
		logger.Info(itemsRemoved, "items totaling", bytesRemoved, "bytes were removed from the cache.", strings.Repeat(" ", 60))
	}

	return nil
}

// TODO: We could use a Modeler that only delivers a message (i.e. SimpleModeler). Use it here and in monitors --decache to report some data in case the standard error is redirected.
