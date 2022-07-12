package scrapePkg

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"fmt"
	"os"
	"strconv"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// HandleScrapeBlaze is called each time around the forever loop prior to calling into
// Blaze to actually scrape the blocks.
func (opts *ScrapeOptions) HandleScrapeBlaze(progressThen *rpcClient.MetaData) (ok bool, err error) {

	if utils.OnOff {
		fmt.Println()
		fmt.Println("----------------------------------------------------------------------------------------------")
		logger.Log(logger.Info, "HandleScrapeBlaze", os.Getenv("TEST_END_SCRAPE"))
	}

	tes := os.Getenv("TEST_END_SCRAPE")
	val, err := strconv.ParseUint(tes, 10, 32)
	if (val != 0 && progressThen.Finalized > val) || err != nil {
		return false, err
	}

	envs := opts.getEnvStr()
	envs = append(envs, opts.Z_8_getSetting(progressThen, "TB_BLAZE_BLOCKCNT"))
	envs = append(envs, opts.Z_8_getSetting(progressThen, "TB_BLAZE_STARTBLOCK"))
	envs = append(envs, opts.Z_8_getSetting(progressThen, "TB_BLAZE_CHAIN"))
	envs = append(envs, opts.Z_8_getSetting(progressThen, "TB_SETTINGS_BLOCKCHANCNT"))
	envs = append(envs, opts.Z_8_getSetting(progressThen, "TB_SETTINGS_ADDRCHANCNT"))
	envs = append(envs, opts.Z_8_getSetting(progressThen, "TB_SETTINGS_APPSPERCHUNK"))
	envs = append(envs, opts.Z_8_getSetting(progressThen, "TB_SETTINGS_UNRIPEDIST"))
	envs = append(envs, opts.Z_8_getSetting(progressThen, "TB_SETTINGS_SNAPTOGRID"))
	envs = append(envs, opts.Z_8_getSetting(progressThen, "TB_SETTINGS_FIRSTSNAP"))
	envs = append(envs, opts.Z_8_getSetting(progressThen, "TB_SETTINGS_ALLOWMISSING"))

	// TODO: BOGUS - TESTING SCRAPING
	if utils.OnOff {
		fmt.Println("Calling with", opts.toCmdLine())
		for _, e := range envs {
			fmt.Println(e)
		}
		fmt.Println(progressThen)
	}

	opts.Globals.Chain = strings.Replace(opts.Z_8_getSetting(progressThen, "TB_BLAZE_CHAIN"), "TB_BLAZE_CHAIN=", "", -1)
	opts.StartBlock, _ = strconv.ParseUint(strings.Replace(opts.Z_8_getSetting(progressThen, "TB_BLAZE_STARTBLOCK"), "TB_BLAZE_STARTBLOCK=", "", -1), 10, 64)
	opts.BlockCnt, _ = strconv.ParseUint(strings.Replace(opts.Z_8_getSetting(progressThen, "TB_BLAZE_BLOCKCNT"), "TB_BLAZE_BLOCKCNT=", "", -1), 10, 64)
	opts.BlockChanCnt, _ = strconv.ParseUint(strings.Replace(opts.Z_8_getSetting(progressThen, "TB_SETTINGS_BLOCKCHANCNT"), "TB_SETTINGS_BLOCKCHANCNT=", "", -1), 10, 64)
	opts.AddrChanCnt, _ = strconv.ParseUint(strings.Replace(opts.Z_8_getSetting(progressThen, "TB_SETTINGS_ADDRCHANCNT"), "TB_SETTINGS_ADDRCHANCNT=", "", -1), 10, 64)

	// TODO: BOGUS - TESTING SCRAPING
	if utils.OnOff {
		logger.Log(logger.Info, "chain:", opts.Globals.Chain)
		logger.Log(logger.Info, "start_block:", opts.StartBlock)
		logger.Log(logger.Info, "block_cnt:", opts.BlockCnt)
		logger.Log(logger.Info, "block_chan_cnt:", opts.BlockChanCnt)
		logger.Log(logger.Info, "addr_chan_cnt:", opts.AddrChanCnt)
	}

	meta, _ := rpcClient.GetMetaData(opts.Globals.Chain, false /* testMode */)
	blazeOpts := BlazeOptions{
		Chain:       opts.Globals.Chain,
		NChannels:   utils.Max(opts.BlockChanCnt, opts.AddrChanCnt),
		NProcessed:  0,
		StartBlock:  opts.StartBlock,
		BlockCount:  opts.BlockCnt,
		RipeBlock:   meta.Latest - utils.Min(meta.Latest, opts.UnripeDist),
		UnripeDist:  opts.UnripeDist,
		RpcProvider: config.GetRpcProvider(opts.Globals.Chain),
	}

	err = blazeOpts.HandleBlaze(meta)
	if err != nil {
		os.RemoveAll(config.GetPathToIndex(opts.Globals.Chain) + "ripe")
		return true, err
	}

	return true, nil
}
