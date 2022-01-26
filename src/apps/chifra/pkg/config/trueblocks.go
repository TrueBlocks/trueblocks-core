// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package config

import (
	"os/user"
	"path"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/spf13/viper"
)

// We keep a map of per-chain configs. Before reading the chain specific
// config, we read the global (top-level) config at trueBlocks.toml. Chain
// config files (if present) appear in ./configs/<chain>/<chain>.toml
// and are merged into the defaults.
var trueBlocksViper = viper.New()
var trueBlocksConfig TrueBlocksConfig

type versionGroup struct {
	Current string `toml:"current"`
}

type settingsGroup struct {
	RpcProvider  string `toml:"rpcProvider"`
	CachePath    string `toml:"cachePath"`
	IndexPath    string `toml:"indexPath"`
	EtherscanKey string `toml:"etherscan_key"`
}

type TrueBlocksConfig struct {
	Version  versionGroup
	Settings settingsGroup
}

// init sets up default values for the given configuration
func init() {
	trueBlocksViper.SetConfigName("trueBlocks")
	trueBlocksViper.SetDefault("Settings.RpcProvider", "http://localhost:8545")
	trueBlocksViper.SetDefault("Settings.CachePath", GetPathToRootConfig()+"cache/")
	trueBlocksViper.SetDefault("Settings.IndexPath", GetPathToRootConfig()+"unchained/")
	trueBlocksViper.SetDefault("Settings.EtherscanKey", "")
}

// readTrueBlocks reads and the configuration located in trueBlocks.toml file. Note
// that this routine is local to the package
func readTrueBlocks() *TrueBlocksConfig {
	if len(trueBlocksConfig.Settings.CachePath) == 0 {
		configPath := GetPathToRootConfig()
		MustReadConfig(trueBlocksViper, &trueBlocksConfig, configPath, false)

		user, _ := user.Current()

		cachePath := trueBlocksConfig.Settings.CachePath
		if len(cachePath) == 0 || cachePath == "<not_set>" {
			cachePath = path.Join(configPath, "cache") + "/"
		}
		cachePath = strings.Replace(cachePath, "$HOME", user.HomeDir, -1)
		cachePath = strings.Replace(cachePath, "~", user.HomeDir, -1)
		trueBlocksConfig.Settings.CachePath = cachePath

		indexPath := trueBlocksConfig.Settings.IndexPath
		if len(indexPath) == 0 || indexPath == "<not_set>" {
			indexPath = path.Join(configPath, "unchained") + "/"
		}
		indexPath = strings.Replace(indexPath, "$HOME", user.HomeDir, -1)
		indexPath = strings.Replace(indexPath, "~", user.HomeDir, -1)
		trueBlocksConfig.Settings.IndexPath = indexPath

		// We establish only the top-level folders here. When we figure out
		// which chain we're on (not until the user tells us on the command line)
		// only then can we complete these paths. At this point these paths
		// only point to the top-levl of the cache or index. Also note that
		// these two calls do not return if they fail, so no need to handle errors
		// TODO: BOGUS-DEFAULTCHAIN
		defaultChains := []string{GetDefaultChain()}
		file.EstablishFolders(trueBlocksConfig.Settings.CachePath, defaultChains)
		file.EstablishFolders(trueBlocksConfig.Settings.IndexPath, defaultChains)
	}

	return &trueBlocksConfig
}

func GetDefaultChain() string {
	// TODO: BOGUS-DEFAULTCHAIN
	return "mainnet"
}

func GetTestChain() string {
	// TODO: BOGUS-DEFAULTCHAIN
	return "mainnet"
}
