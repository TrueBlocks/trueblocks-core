// TODO: BOGUS - THINGS TO DO:
// TODO: BOGUS - CLEANUP BETWEEN RUNS IF THINGS DON'T WORK OUT AND SOMETHING HAD TO QUIT EARLY
// TODO: BOGUS - MAKE SURE WE'RE NOT RUNNING IF ACCTSCRAPE (OR ANYTHING ELSE?) IS RUNNINGs
// TODO: BOGUS - RESPOND WELL TO RUNNING AGAINST NON-ARCHIVE NODES see isArchiveNode() in C++ code
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
	"github.com/ethereum/go-ethereum/common/hexutil"
)

func (opts *ScrapeOptions) preLoop(progressThen *rpcClient.MetaData) (bool, error) {
	logger.Log(logger.Info, "PreLoop")
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
		index.WriteChunk(opts.Globals.Chain, path, apps, len(allocs))
		// TODO: BOGUS - PINNING WHEN WRITING IN GOLANG
		// rng := "000000000-000000000"
		// newPinsFn := config.GetPathToCache(opts.Globals.Chain) + "tmp/chunks_created.txt"
		// file.AppendToAsciiFile(newPinsFn, rng+"\n")
	}
	return true, nil
}
