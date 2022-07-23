package scrapePkg

import (
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config/scrape"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
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

type ScraperState struct {
	StartBlock    uint64
	NRecsNow      uint64
	NRecsThen     uint64
	NAppsPerChunk uint64
	BlockCount    uint64
}

func NewScraperState(sB, nN, nT, nA, bC uint64) ScraperState {
	ss := ScraperState{}
	ss.StartBlock = sB
	ss.NRecsNow = nN
	ss.NRecsThen = nT
	ss.NAppsPerChunk = nA
	ss.BlockCount = bC
	return ss
}

func (ss *ScraperState) Report() {
	if ss.NRecsNow == ss.NRecsThen {
		logger.Log(logger.Info, "No new blocks...")
	} else {
		need := uint64(0)
		if ss.NAppsPerChunk >= ss.NRecsNow {
			need = ss.NAppsPerChunk - ss.NRecsNow
		}
		seen := ss.NRecsNow - ss.NRecsThen
		pct := float64(ss.NRecsNow) / float64(ss.NAppsPerChunk)
		pBlk := float64(seen) / float64(ss.BlockCount)
		format := "Block {%d}: have {%d} addrs of {%d} ({%0.1f}). Need {%d} more. Found {%d} records ({%0.2f})."
		msg := fmt.Sprintf(format, (ss.StartBlock + ss.BlockCount - 1), ss.NRecsNow, ss.NAppsPerChunk, (pct * 100.00), need, seen, pBlk)
		msg = strings.Replace(msg, "{", colors.Green, -1)
		msg = strings.Replace(msg, "}", colors.Off, -1)
		logger.Log(logger.Info, msg)
	}
}
