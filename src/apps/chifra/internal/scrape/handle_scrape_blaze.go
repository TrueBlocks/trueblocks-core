package scrapePkg

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"fmt"
	"os"
	"sort"
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// HandleScrapeBlaze is called each time around the forever loop prior to calling into
// Blaze to actually scrape the blocks.
func (opts *ScrapeOptions) HandleScrapeBlaze(progressThen *rpcClient.MetaData) (ok bool, err error) {

	// Quit early if we're testing... TODO: BOGUS - REMOVE THIS
	tes := os.Getenv("TEST_END_SCRAPE")
	if tes != "" {
		val, err := strconv.ParseUint(tes, 10, 32)
		if (val != 0 && progressThen.Staging > val) || err != nil {
			return false, err
		}
	}

	// We always start one after where we last left off
	opts.StartBlock = utils.Max(progressThen.Ripe, utils.Max(progressThen.Staging, progressThen.Finalized)) + 1
	if (opts.StartBlock + opts.BlockCnt) > progressThen.Latest {
		opts.BlockCnt = (progressThen.Latest - opts.StartBlock)
	}

	// TODO: BOGUS - TESTING SCRAPING
	if utils.DebuggingOn {
		fmt.Println()
		fmt.Println("----------------------------------------------------------------------------------------------")
		logger.Log(logger.Info, "Handle ScrapeBlaze", os.Getenv("TEST_END_SCRAPE"))

		envs := opts.getEnvStrings(progressThen)
		fmt.Println("Calling with", opts.toCmdLine())
		for _, e := range envs {
			fmt.Println(e)
		}
		fmt.Println(progressThen)

		logger.Log(logger.Info, "chain:", opts.Globals.Chain)
		logger.Log(logger.Info, "start_block:", opts.StartBlock)
		logger.Log(logger.Info, "block_cnt:", opts.BlockCnt)
		logger.Log(logger.Info, "block_chan_cnt:", opts.BlockChanCnt)
		logger.Log(logger.Info, "addr_chan_cnt:", opts.AddrChanCnt)
	}

	meta, _ := rpcClient.GetMetaData(opts.Globals.Chain, false /* testMode */)
	blazeOpts := BlazeOptions{
		Chain:         opts.Globals.Chain,
		NChannels:     utils.Max(opts.BlockChanCnt, opts.AddrChanCnt),
		NProcessed:    0,
		StartBlock:    opts.StartBlock,
		BlockCount:    opts.BlockCnt,
		RipeBlock:     meta.Latest - utils.Min(meta.Latest, opts.UnripeDist),
		UnripeDist:    opts.UnripeDist,
		RpcProvider:   config.GetRpcProvider(opts.Globals.Chain),
		AppearanceMap: make(index.AddressAppearanceMap, 500000),
		TsArray:       make([]tslib.Timestamp, opts.BlockCnt),
	}

	_, err = blazeOpts.HandleBlaze(meta)
	// Even though we may have errored, we want to process whatever data we know we
	// have and is in a "good" state, such as timestamps
	blazeOpts.CleanupPostScrape()
	if utils.DebuggingOn {
		logger.Log(logger.Info, "Size of AppMap:", len(blazeOpts.AppearanceMap))
	}
	if err != nil {
		os.RemoveAll(config.GetPathToIndex(opts.Globals.Chain) + "ripe")
		return true, err
	}

	return true, nil
}

func (opts *BlazeOptions) CleanupPostScrape() {
	sort.Slice(opts.TsArray, func(i, j int) bool {
		return opts.TsArray[i].Bn < opts.TsArray[j].Bn
	})

	tslib.DeCache(opts.Chain)
	nTimestamps, _ := tslib.NTimestamps(opts.Chain)
	height := uint32(nTimestamps - 1)
	if nTimestamps == 0 {
		height = 0
	}

	first := 0
	for i, ts := range opts.TsArray {
		if ts.Bn > height {
			first = i
			break
		}
	}

	// TODO: CLEAN THIS UP
	if utils.DebuggingOn {
		for _, ts := range opts.TsArray[first:] {
			fmt.Println(ts)
		}
	}

	tslib.Append(opts.Chain, opts.TsArray[first:])
}
