package scrapePkg

// TODO: BOGUS - MULTI-CHAIN SPECIFIC SLEEPING IS
// TODO: BOGUS - THINGS TO DO:
// TODO: BOGUS - CLEANUP BETWEEN RUNS IF THINGS DON'T WORK OUT AND SOMETHING HAD TO QUIT EARLY
// TODO: BOGUS - MAKE SURE WE'RE NOT RUNNING IF ACCTSCRAPE (OR ANYTHING ELSE?) IS RUNNINGs
// TODO: BOGUS - RESPOND WELL TO RUNNING AGAINST NON-ARCHIVE NODES see isArchiveNode() in C++ code
// TODO: BOGUS - RETRY ON FAILURE OF BLAZE - SEE NOTES BELOW
// TODO: BOGUS - THINGS TO DO:
// TODO: BOGUS - CLEANUP BETWEEN RUNS IF THINGS DON'T WORK OUT AND SOMETHING HAD TO QUIT EARLY
// TODO: BOGUS - MAKE SURE WE'RE NOT RUNNING IF ACCTSCRAPE (OR ANYTHING ELSE?) IS RUNNINGs
// TODO: BOGUS - RESPOND WELL TO RUNNING AGAINST NON-ARCHIVE NODES see isArchiveNode() in C++ code

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"os"
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *ScrapeOptions) HandleScrape() error {
	if ok, err := opts.HandlePrepare(); !ok || err != nil {
		return err
	}

	for {
		progress, err := rpcClient.GetMetaData(opts.Globals.Chain, opts.Globals.TestMode)
		if err != nil {
			return err
		}

		// Quit early if we're testing... TODO: BOGUS - REMOVE THIS
		tes := os.Getenv("TEST_END_SCRAPE")
		if tes != "" {
			val, err := strconv.ParseUint(tes, 10, 32)
			if (val != 0 && progress.Staging > val) || err != nil {
				logger.Exit("HandleScrapeBlaze - Quitting early", err)
				return err
			}
		}

		// We always start one after where we last left off
		opts.StartBlock = utils.Max(progress.Ripe, utils.Max(progress.Staging, progress.Finalized)) + 1
		if (opts.StartBlock + opts.BlockCnt) > progress.Latest {
			opts.BlockCnt = (progress.Latest - opts.StartBlock)
		}

		// '28' behind head unless head is less or equal to than '28', then head
		ripe := progress.Latest
		if ripe > opts.UnripeDist {
			ripe = progress.Latest - opts.UnripeDist
		}

		blazeOpts := BlazeOptions{
			Chain:         opts.Globals.Chain,
			NChannels:     utils.Max(opts.BlockChanCnt, opts.AddrChanCnt),
			NProcessed:    0,
			StartBlock:    opts.StartBlock,
			BlockCount:    opts.BlockCnt,
			RipeBlock:     ripe,
			UnripeDist:    opts.UnripeDist,
			RpcProvider:   config.GetRpcProvider(opts.Globals.Chain),
			AppearanceMap: make(index.AddressAppearanceMap, opts.AppsPerChunk),
			TsArray:       make([]tslib.Timestamp, 0, opts.BlockCnt),
			ProcessedMap:  make(map[int]bool, opts.BlockCnt),
		}

		if ok, err := opts.HandleScrapeBlaze(progress, &blazeOpts); !ok || err != nil {
			if !ok {
				break
			}
			goto PAUSE
		}

		if ok, err := opts.HandleScrapeConsolidate(progress, &blazeOpts); !ok || err != nil {
			if !ok {
				break
			}
			goto PAUSE
		}

		// Clean up after this run of the blockScraper
		if ok, err := opts.HandleScrapePin(); !ok || err != nil {
			if !ok {
				break
			}
			goto PAUSE
		}

	PAUSE:
		opts.Pause(progress)
	}

	return nil
}

// Used for debugging, will be removed
var Debugging = file.FileExists("./testing")
var ForceFail = file.FileExists("./forcefail")
