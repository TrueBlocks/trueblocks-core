package config

import (
	"fmt"
	"os"
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/configtypes"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/version"
)

func pathToChainConfigFile(chain, fileName string) string {
	return filepath.Join(PathToRootConfig(), "config", chain, fileName)
}

var minVersionStr = "v2.0.0-release"

// migrate upgrades the config files to the latest versions if necessary
func migrate(currentVer version.Version) error {
	minVersion := version.NewVersion(minVersionStr)
	if currentVer.Uint64() >= minVersion.Uint64() {
		return nil
	}
	// note that this routine does not return if it gets this far.

	var cfg configtypes.Config
	configFile := PathToConfigFile()
	if err := ReadToml(configFile, &cfg); err != nil {
		return err
	}

	vers := version.NewVersion("v1.0.0-release")
	if currentVer.Uint64() < vers.Uint64() {
		for chain := range cfg.Chains {
			ch := cfg.Chains[chain]
			ch.Chain = chain
			scrape := configtypes.ScrapeSettings{
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
	}

	vers = version.NewVersion("v2.0.0-release")
	if currentVer.Uint64() < vers.Uint64() {
		pinning := configtypes.PinningGroup{
			LocalPinUrl:  "http://localhost:5001",
			RemotePinUrl: "https://api.pinata.cloud/pinning/pinFileToIPFS",
		}
		if cfg.Settings.DefaultGateway != "" {
			pinning.GatewayUrl = cfg.Settings.DefaultGateway
		} else {
			pinning.GatewayUrl = defaultIpfsGateway
		}
		cfg.Settings.DefaultGateway = ""
		cfg.Pinning = pinning
	}

	// Re-write the file (after making a backup) with the new version
	_, _ = file.Copy(configFile+".bak", configFile)
	_ = cfg.WriteFile(configFile, minVersion) // updates the version

	msg := "Your configuration files were upgraded to {%s}. Rerun your command."
	logger.Panic(colors.Colored(fmt.Sprintf(msg, minVersion.String())))

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

func MergeScrapeConfig(fn string, scrape *configtypes.ScrapeSettings) error {
	var sCfg OldScrape
	if err := ReadToml(fn, &sCfg); err != nil {
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
