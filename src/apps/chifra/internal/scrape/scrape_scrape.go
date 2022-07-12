package scrapePkg

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

// Scrape calls into the block scraper to (a) call Blaze and (b) consolidate if applicable
func (opts *ScrapeOptions) Y_3_scrape(progressThen *rpcClient.MetaData) (bool, error) {
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
	fmt.Println("Calling with", opts.toCmdLine())
	for _, e := range envs {
		fmt.Println(e)
	}
	fmt.Println(progressThen)

	opts.Globals.Chain = strings.Replace(opts.Z_8_getSetting(progressThen, "TB_BLAZE_CHAIN"), "TB_BLAZE_CHAIN=", "", -1)
	opts.StartBlock, _ = strconv.ParseUint(strings.Replace(opts.Z_8_getSetting(progressThen, "TB_BLAZE_STARTBLOCK"), "TB_BLAZE_STARTBLOCK=", "", -1), 10, 64)
	opts.BlockCnt, _ = strconv.ParseUint(strings.Replace(opts.Z_8_getSetting(progressThen, "TB_BLAZE_BLOCKCNT"), "TB_BLAZE_BLOCKCNT=", "", -1), 10, 64)
	opts.BlockChanCnt, _ = strconv.ParseUint(strings.Replace(opts.Z_8_getSetting(progressThen, "TB_SETTINGS_BLOCKCHANCNT"), "TB_SETTINGS_BLOCKCHANCNT=", "", -1), 10, 64)
	opts.AddrChanCnt, _ = strconv.ParseUint(strings.Replace(opts.Z_8_getSetting(progressThen, "TB_SETTINGS_ADDRCHANCNT"), "TB_SETTINGS_ADDRCHANCNT=", "", -1), 10, 64)

	logger.Log(logger.Info, "chain:", opts.Globals.Chain)
	logger.Log(logger.Info, "start_block:", opts.StartBlock)
	logger.Log(logger.Info, "block_cnt:", opts.BlockCnt)
	logger.Log(logger.Info, "block_chan_cnt:", opts.BlockChanCnt)
	logger.Log(logger.Info, "addr_chan_cnt:", opts.AddrChanCnt)

	err := opts.HandleScrapeBlaze()
	if err != nil {
		os.RemoveAll(config.GetPathToIndex(opts.Globals.Chain) + "ripe")
		return true, err
	}

	return true, opts.Globals.PassItOn("blockScrape", opts.Globals.Chain, opts.toCmdLine(), envs)
}

func (opts *ScrapeOptions) Z_8_getSetting(meta *rpcClient.MetaData, which string) string {
	settings := config.GetBlockScrapeSettings(opts.Globals.Chain)
	start := utils.Max(meta.Ripe, utils.Max(meta.Staging, meta.Finalized)) + 1
	value := ""

	switch which {

	case "TB_BLAZE_BLOCKCNT":
		blockCnt := opts.BlockCnt
		if (start + blockCnt) > meta.Latest {
			blockCnt = (meta.Latest - start)
		}
		value = fmt.Sprintf("%d", blockCnt)

	case "TB_BLAZE_STARTBLOCK":
		value = fmt.Sprintf("%d", start)

	case "TB_BLAZE_CHAIN":
		value = opts.Globals.Chain

	case "TB_SETTINGS_BLOCKCHANCNT":
		val := uint64(settings.Block_chan_cnt)
		if opts.BlockChanCnt != val {
			val = opts.BlockChanCnt
		}
		value = fmt.Sprintf("%d", val)

	case "TB_SETTINGS_ADDRCHANCNT":
		val := uint64(settings.Addr_chan_cnt)
		if opts.AddrChanCnt != val {
			val = opts.AddrChanCnt
		}
		value = fmt.Sprintf("%d", val)

	case "TB_SETTINGS_APPSPERCHUNK":
		perChunk := settings.Apps_per_chunk
		if (opts.Globals.Chain == "mainnet") && perChunk == 200000 {
			perChunk = 2000000
		}
		value = fmt.Sprintf("%d", perChunk)

	case "TB_SETTINGS_FIRSTSNAP":
		firstSnap := settings.First_snap
		if opts.Globals.Chain == "mainnet" && firstSnap == 0 {
			firstSnap = 2250000
		}
		value = fmt.Sprintf("%d", firstSnap)

	case "TB_SETTINGS_UNRIPEDIST":
		value = fmt.Sprintf("%d", settings.Unripe_dist)

	case "TB_SETTINGS_SNAPTOGRID":
		value = fmt.Sprintf("%d", settings.Snap_to_grid)

	case "TB_SETTINGS_ALLOWMISSING":
		value = fmt.Sprintf("%t", settings.Allow_missing)
	}

	return which + "=" + value
}
