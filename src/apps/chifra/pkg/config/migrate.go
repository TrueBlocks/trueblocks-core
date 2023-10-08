package config

import (
	"fmt"
	"os"
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
)

// migrate will upgrade the config files to the latest versions
func migrate(oldVersion, newVersion version.Version) error {
	pathToConfig := PathToRootConfig()
	pathToChainConfigFile := func(chain, fileName string) string {
		return filepath.Join(pathToConfig, "config", chain, fileName)
	}

	fn := pathToConfig + "trueBlocks.toml"
	var cfg ConfigFile
	if err := readFile(fn, &cfg); err != nil {
		return err
	}

	for chain := range cfg.Chains {
		ch := cfg.Chains[chain]
		ch.Chain = chain
		scrape := ScrapeSettings{
			AppsPerChunk: 2000000,
			SnapToGrid:   250000,
			FirstSnap:    2000000,
			UnripeDist:   28,
			AllowMissing: false,
			ChannelCount: 20,
		}
		if chain == "mainnet" {
			scrape.SnapToGrid = 100000
			scrape.FirstSnap = 2300000
		}

		fn := pathToChainConfigFile(chain, "blockScrape.toml")
		if file.FileExists(fn) {
			_ = MergeScrapeConfig(fn, &scrape)
			_ = os.Remove(fn)
		}
		ch.Scrape = scrape
		cfg.Chains[chain] = ch

		oldPath := pathToChainConfigFile(chain, "manifest.json")
		newPath := filepath.Join(PathToIndex(chain), "manifest.json")
		if file.FileExists(oldPath) {
			_, _ = file.Copy(newPath, oldPath)
			_ = os.Remove(oldPath)
		}
	}

	// Re-write the file (after making a backup) with the new version
	_, _ = file.Copy(fn+".bak", fn)
	_ = cfg.writeFile(fn, newVersion) // updates the version
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

func MergeScrapeConfig(fn string, scrape *ScrapeSettings) error {
	var sCfg OldScrape
	if err := readFile(fn, &sCfg); err != nil {
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
