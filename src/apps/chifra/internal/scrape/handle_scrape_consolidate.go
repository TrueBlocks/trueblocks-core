package scrapePkg

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// HandleScrapeConsolidate calls into the block scraper to (a) call Blaze and (b) consolidate if applicable
func (opts *ScrapeOptions) HandleScrapeConsolidate(progressThen *rpcClient.MetaData) (ok bool, err error) {
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
