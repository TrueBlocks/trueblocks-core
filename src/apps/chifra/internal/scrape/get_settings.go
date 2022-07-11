package scrapePkg

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *ScrapeOptions) Z_8_getSetting(meta *rpcClient.MetaData, which string) string {
	settings := config.ReadBlockScrape(opts.Globals.Chain).Settings
	start := utils.Max(meta.Ripe, utils.Max(meta.Staging, meta.Finalized)) + 1
	value := ""

	switch which {

	case "TB_SETTINGS_BLOCKCNT":
		blockCnt := opts.BlockCnt
		if (start + blockCnt) > meta.Latest {
			blockCnt = (meta.Latest - start)
		}
		value = fmt.Sprintf("%d", blockCnt)

	case "TB_SETTINGS_STARTBLOCK":
		value = fmt.Sprintf("%d", start)

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
