package scrapePkg

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/ethereum/go-ethereum/common/hexutil"
)

// HandlePrepare performs actions that need to happen prior to entering the forever loop. Returns
// true if the processing should continue, false otherwise. Currently, the only thing to do
// is write the zero block Index Chunk / Bloom filter pair if it doesn't exist.
func (opts *ScrapeOptions) HandlePrepare(progressThen *rpcClient.MetaData) (ok bool, err error) {
	if utils.DebuggingOn {
		logger.Log(logger.Info, "HandlePrepare")
	}

	pathObj := cache.NewCachePath(opts.Globals.Chain, cache.Index_Bloom)
	bloomPath := pathObj.GetFullPath("000000000-000000000")
	if file.FileExists(bloomPath) {
		// The file already exists, nothing to do
		return true, nil
	}

	allocs, err := names.LoadPrefunds(opts.Globals.Chain)
	if err != nil {
		return false, err
	}

	appMap := make(index.AddressAppearanceMap, len(allocs))
	for i, alloc := range allocs {
		addr := hexutil.Encode(alloc.Address.Bytes()) // a lowercase string
		appMap[addr] = append(appMap[addr], index.AppearanceRecord{
			BlockNumber:   0,
			TransactionId: uint32(i),
		})
	}
	_, err = index.WriteChunk(opts.Globals.Chain, index.ToIndexPath(bloomPath), appMap, len(allocs))
	if err != nil {
		return false, err
	}
	logger.Log(logger.Info, "Size of appMap:", len(appMap))

	// TODO: BOGUS - PINNING WHEN WRITING IN GOLANG
	if opts.Pin {
		rng := "000000000-000000000"
		newPinsFn := config.GetPathToCache(opts.Globals.Chain) + "tmp/chunks_created.txt"
		file.AppendToAsciiFile(newPinsFn, rng+"\n")
	}

	// In this special case, we need to postScrape here since we've created an index file
	return opts.HandleScrapePin(progressThen)
}
