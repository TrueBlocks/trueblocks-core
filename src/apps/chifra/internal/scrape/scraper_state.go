package scrapePkg

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config/scrape"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *ScrapeOptions) getEnvStrings(progressThen *rpcClient.MetaData) []string {
	envs := opts.getEnvStr()
	envs = append(envs, opts.Z_8_getSetting(progressThen, "TB_BLAZE_BLOCKCNT"))
	envs = append(envs, opts.Z_8_getSetting(progressThen, "TB_BLAZE_STARTBLOCK"))
	envs = append(envs, opts.Z_8_getSetting(progressThen, "TB_SETTINGS_BLOCKCHANCNT"))
	envs = append(envs, opts.Z_8_getSetting(progressThen, "TB_SETTINGS_ADDRCHANCNT"))
	envs = append(envs, opts.Z_8_getSetting(progressThen, "TB_SETTINGS_APPSPERCHUNK"))
	envs = append(envs, opts.Z_8_getSetting(progressThen, "TB_SETTINGS_UNRIPEDIST"))
	envs = append(envs, opts.Z_8_getSetting(progressThen, "TB_SETTINGS_SNAPTOGRID"))
	envs = append(envs, opts.Z_8_getSetting(progressThen, "TB_SETTINGS_FIRSTSNAP"))
	envs = append(envs, opts.Z_8_getSetting(progressThen, "TB_SETTINGS_ALLOWMISSING"))
	return envs
}

func (opts *ScrapeOptions) Z_8_getSetting(progressThen *rpcClient.MetaData, which string) string {
	settings := scrape.GetSettings(opts.Globals.Chain)
	value := ""

	switch which {

	case "TB_BLAZE_BLOCKCNT":
		start := utils.Max(progressThen.Ripe, utils.Max(progressThen.Staging, progressThen.Finalized)) + 1
		blockCnt := opts.BlockCnt
		if (start + blockCnt) > progressThen.Latest {
			blockCnt = (progressThen.Latest - start)
		}
		value = fmt.Sprintf("%d", blockCnt)

	case "TB_BLAZE_STARTBLOCK":
		start := utils.Max(progressThen.Ripe, utils.Max(progressThen.Staging, progressThen.Finalized)) + 1
		value = fmt.Sprintf("%d", start)

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

	case "TB_SETTINGS_BLOCKCHANCNT":
		value = fmt.Sprintf("%d", settings.Block_chan_cnt)

	case "TB_SETTINGS_ADDRCHANCNT":
		value = fmt.Sprintf("%d", settings.Addr_chan_cnt)

	case "TB_SETTINGS_UNRIPEDIST":
		value = fmt.Sprintf("%d", settings.Unripe_dist)

	case "TB_SETTINGS_SNAPTOGRID":
		value = fmt.Sprintf("%d", settings.Snap_to_grid)

	case "TB_SETTINGS_ALLOWMISSING":
		value = fmt.Sprintf("%t", settings.Allow_missing)
	}

	return which + "=" + value
}
