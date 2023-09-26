package upgrade

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
)

// upgradeConfigs will upgrade the config files to the latest versions
func UpgradeConfigs(newVersion version.Version) error {
	fn := config.PathToRootConfig() + "trueBlocks.toml"
	var cfg config.ConfigFile
	if err := config.ReadConfigFile(fn, &cfg); err != nil {
		return err
	}

	for chain, group := range cfg.Chains {
		group.Chain = chain
		ch := cfg.Chains[chain]
		scrape := config.ScrapeSettings{
			AppsPerChunk: 500000,
			SnapToGrid:   100000,
			FirstSnap:    500000,
			UnripeDist:   28,
			AllowMissing: false,
			ChannelCount: 20,
		}
		if chain == "mainnet" {
			scrape.AppsPerChunk = 2000000
			scrape.FirstSnap = 2300000
		}

		fn := config.PathToRootConfig() + "config/" + chain + "/blockScrape.toml"
		if file.FileExists(fn) {
			_ = MergeScrapeConfig(fn, &scrape)
		}
		ch.Scrape = scrape
		cfg.Chains[chain] = ch
	}

	// Re-write the file (after making a backup) with the new version
	_, _ = file.Copy(fn, fn+".bak")
	_ = cfg.WriteConfigFile(fn) // updates the version
	logger.Fatal(colors.Colored(fmt.Sprintf("Your configuration files were upgraded to {%s}. Rerun your command.", newVersion.String())))

	return nil
}

type oldScrapeGroup struct {
	AppsPerChunk uint64 `toml:"apps_per_chunk"`
	SnapToGrid   uint64 `toml:"snap_to_grid"`
	FirstSnap    uint64 `toml:"first_snap"`
	UnripeDist   uint64 `toml:"unripe_dist"`
	AllowMissing bool   `toml:"allow_missing"`
	ChannelCount uint64 `toml:"channel_count"`
}

type OldScrape struct {
	Settings oldScrapeGroup `toml:"settings"`
}

func MergeScrapeConfig(fn string, scrape *config.ScrapeSettings) error {
	var sCfg OldScrape
	if err := config.ReadConfigFile(fn, &sCfg); err != nil {
		return err
	}
	if sCfg.Settings.AppsPerChunk > 0 {
		scrape.AppsPerChunk = sCfg.Settings.AppsPerChunk
	}
	if sCfg.Settings.SnapToGrid > 0 {
		scrape.SnapToGrid = sCfg.Settings.SnapToGrid
	}
	if sCfg.Settings.FirstSnap > 0 {
		scrape.FirstSnap = sCfg.Settings.FirstSnap
	}
	if sCfg.Settings.UnripeDist > 0 {
		scrape.UnripeDist = sCfg.Settings.UnripeDist
	}
	if sCfg.Settings.ChannelCount > 0 {
		scrape.ChannelCount = sCfg.Settings.ChannelCount
	}
	scrape.AllowMissing = sCfg.Settings.AllowMissing
	return nil
}
