package scrapePkg

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
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

	return true, opts.Globals.PassItOn("blockScrape", opts.Globals.Chain, opts.toCmdLine(), envs)
}
