package scrapePkg

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config/scrape"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *ScrapeOptions) getEnvStrings(progressThen *rpcClient.MetaData) []string {
	envs := opts.getEnvStr()
	envs = append(envs, opts.Z_8_getSetting(progressThen, "TB_BLAZE_STARTBLOCK"))
	envs = append(envs, opts.Z_8_getSetting(progressThen, "TB_SETTINGS_APPSPERCHUNK"))
	envs = append(envs, opts.Z_8_getSetting(progressThen, "TB_SETTINGS_SNAPTOGRID"))
	envs = append(envs, opts.Z_8_getSetting(progressThen, "TB_SETTINGS_FIRSTSNAP"))
	return envs
}

func (opts *ScrapeOptions) Z_8_getSetting(progressThen *rpcClient.MetaData, which string) string {
	settings := scrape.GetSettings(opts.Globals.Chain)
	switch which {
	case "TB_BLAZE_STARTBLOCK":
		start := utils.Max(progressThen.Ripe, utils.Max(progressThen.Staging, progressThen.Finalized)) + 1
		return which + "=" + fmt.Sprintf("%d", start)

	case "TB_SETTINGS_APPSPERCHUNK":
		return which + "=" + fmt.Sprintf("%d", settings.Apps_per_chunk)

	case "TB_SETTINGS_FIRSTSNAP":
		return which + "=" + fmt.Sprintf("%d", settings.First_snap)

	case "TB_SETTINGS_SNAPTOGRID":
		return which + "=" + fmt.Sprintf("%d", settings.Snap_to_grid)
	}
	return ""
}
