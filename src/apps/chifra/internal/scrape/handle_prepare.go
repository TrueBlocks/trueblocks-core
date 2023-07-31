package scrapePkg

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/prefunds"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/ethereum/go-ethereum/common/hexutil"
)

// HandlePrepare performs actions that need to happen prior to entering the forever loop. Returns
// true if the processing should continue, false otherwise. Currently, the only thing to do
// is write the zero block Index Chunk / Bloom filter pair if it doesn't exist.
func (opts *ScrapeOptions) HandlePrepare(progressThen *rpcClient.MetaData, blazeOpts *BlazeOptions) (ok bool, err error) {
	// We always clean the temporary folders (other than staging) when starting
	index.CleanTemporaryFolders(config.GetPathToIndex(opts.Globals.Chain), false)

	bloomPath := config.GetPathToIndex(opts.Globals.Chain) + "blooms/000000000-000000000.bloom"
	if file.FileExists(bloomPath) {
		// The file already exists, nothing to do
		return true, nil
	}

	prefundPath := filepath.Join(config.GetPathToChainConfig(opts.Globals.Chain), "allocs.csv")
	prefunds, err := prefunds.LoadPrefunds(opts.Globals.Chain, prefundPath, nil)
	if err != nil {
		return false, err
	}

	appMap := make(index.AddressAppearanceMap, len(prefunds))
	for i, prefund := range prefunds {
		addr := hexutil.Encode(prefund.Address.Bytes()) // a lowercase string
		appMap[addr] = append(appMap[addr], index.AppearanceRecord{
			BlockNumber:   0,
			TransactionId: uint32(i),
		})
	}

	array := []tslib.TimestampRecord{}
	array = append(array, tslib.TimestampRecord{
		Bn: uint32(0),
		Ts: uint32(rpcClient.GetBlockTimestamp(opts.Globals.Chain, utils.PointerOf(uint64(0)))),
	})
	tslib.Append(opts.Globals.Chain, array)

	logger.Info("Writing block zero allocations for", len(prefunds), "prefunds, nAddresses:", len(appMap))
	indexPath := cache.ToIndexPath(bloomPath)
	if report, err := index.WriteChunk(opts.Globals.Chain, indexPath, appMap, len(prefunds), opts.Pin, opts.Remote); err != nil {
		return false, err
	} else if report == nil {
		logger.Fatal("Should not happen, write chunk returned empty report")
	} else {
		report.Snapped = true // assumes block zero is a snap to grid (which it is in a sense)
		report.Report()
	}

	return true, nil
}
