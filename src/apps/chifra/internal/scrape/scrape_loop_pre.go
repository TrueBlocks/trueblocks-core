package scrapePkg

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/ethereum/go-ethereum/common/hexutil"
)

// preLoop does processing prior to entering the forever loop
func (opts *ScrapeOptions) Y_1_preLoop(progressThen *rpcClient.MetaData) (bool, error) {
	// TODO: BOGUS - TESTING SCRAPING
	if utils.OnOff {
		logger.Log(logger.Info, "PreLoop")
	}

	path := config.GetPathToIndex(opts.Globals.Chain) + fmt.Sprintf("finalized/%09d-%09d", 0, 0) + ".bin"
	if !file.FileExists(path) {
		allocs, err := names.LoadPrefunds(opts.Globals.Chain)
		if err != nil {
			return true, err
		}

		nExpected := opts.AppsPerChunk + uint64(float64(opts.AppsPerChunk)*1.2)
		apps := make(index.AddressAppearanceMap, nExpected)
		for i, alloc := range allocs {
			addr := hexutil.Encode(alloc.Address.Bytes()) // a lowercase string
			apps[addr] = append(apps[addr], index.AppearanceRecord{
				BlockNumber:   0,
				TransactionId: uint32(i),
			})
		}
		// nWritten, err
		_, err = index.WriteChunk(opts.Globals.Chain, path, apps, len(allocs), opts.Pin)
		if err != nil {
			return false, err
		}
		opts.Y_4_postScrape(progressThen)
	}
	return true, nil
}
