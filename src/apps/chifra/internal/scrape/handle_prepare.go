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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/ethereum/go-ethereum/common/hexutil"
)

// HandlePrepare performs actions that need to happen prior to entering the forever loop. Returns
// true if the processing should continue, false otherwise. Currently, the only thing to do
// is write the zero block Index Chunk / Bloom filter pair if it doesn't exist.
func (opts *ScrapeOptions) HandlePrepare() (ok bool, err error) {
	// We always clean the temporary folders (other than staging) when starting
	index.CleanTemporaryFolders(config.GetPathToIndex(opts.Globals.Chain), false)

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

	ts := uint32(rpcClient.GetBlockTimestamp(config.GetRpcProvider(opts.Globals.Chain), uint64(0)))
	if ts == 0 {
		// some node return a zero timestamp for the zero block, use block 1 minus 13 in that case
		ts = uint32(rpcClient.GetBlockTimestamp(config.GetRpcProvider(opts.Globals.Chain), uint64(1)))
		// TODO: BOGUS - CHAIN SPECIFIC
		ts -= 13
	}

	logger.Log(logger.Info, "Writing block zero allocations for", len(allocs), "allocs, nAddresses:", len(appMap))
	_, err = index.WriteChunk(opts.Globals.Chain, index.ToIndexPath(bloomPath), appMap, len(allocs), -1)
	if err != nil {
		return false, err
	}

	array := []tslib.Timestamp{}
	array = append(array, tslib.Timestamp{
		Bn: uint32(0),
		Ts: ts,
	})
	tslib.Append(opts.Globals.Chain, array)

	// TODO: BOGUS - CAN'T SEND NILS
	return opts.HandleScrapePin(nil, nil)
}
