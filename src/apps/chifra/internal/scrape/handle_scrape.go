package scrapePkg

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"os"
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config/scrape"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// TODO: We should repond to non-tracing (i.e. Geth) nodes better
// TODO: Make sure we're not running acctScrape and/or pause if it's running
func (opts *ScrapeOptions) HandleScrape() error {
	settings, _ := scrape.GetSettings(opts.Globals.Chain, &opts.Settings)
	if ok, err := opts.HandlePrepare(); !ok || err != nil {
		return err
	}

	for {
		progress, err := rpcClient.GetMetaData(opts.Globals.Chain, opts.Globals.TestMode)
		if err != nil {
			return err
		}

		// TODO: This can be removed at some point in the future
		if os.Getenv("TEST_END_SCRAPE") != "" {
			val, err := strconv.ParseUint(os.Getenv("TEST_END_SCRAPE"), 10, 32)
			if err != nil {
				return err
			}
			if val > 0 && progress.Staging > val {
				index.CleanTemporaryFolders(config.GetPathToIndex(opts.Globals.Chain), false)
				logger.Log(logger.Info, "Finished processing for testing")
				return nil
			}
		}

		// We always start one after where we last left off
		opts.StartBlock = utils.Max(progress.Ripe, utils.Max(progress.Staging, progress.Finalized)) + 1
		if (opts.StartBlock + opts.BlockCnt) > progress.Latest {
			opts.BlockCnt = (progress.Latest - opts.StartBlock)
		}

		// 'UnripeDist' behind head unless head is less or equal to than 'UnripeDist', then head
		ripe := progress.Latest
		if ripe > settings.Unripe_dist {
			ripe = progress.Latest - settings.Unripe_dist
		}

		blazeOpts := BlazeOptions{
			Chain:         opts.Globals.Chain,
			NChannels:     settings.Channel_count,
			NProcessed:    0,
			StartBlock:    opts.StartBlock,
			BlockCount:    opts.BlockCnt,
			RipeBlock:     ripe,
			UnripeDist:    settings.Unripe_dist,
			RpcProvider:   config.GetRpcProvider(opts.Globals.Chain),
			AppearanceMap: make(index.AddressAppearanceMap, settings.Apps_per_chunk),
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
