// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package config

import (
	"os/user"
	"path"
	"strings"

	"github.com/spf13/viper"
)

// TODO: BOGUS - THIS IS OKAY SINCE THERE'S ONLY ONE OF THESE
var trueBlocksViper = viper.New()
var trueBlocksRead = false
var cachedTrueBlocksConfig TrueBlocksConfig

type trueBlocksVersion struct {
	Current string
}

type trueBlocksSettings struct {
	RpcProvider  string `toml:"rpcProvider"`
	CachePath    string `toml:"cachePath"`
	IndexPath    string `toml:"indexPath"`
	EtherscanKey string `toml:"etherscan_key"`
	DefaultChain string `toml:"default_chain"`
}

type TrueBlocksConfig struct {
	Version  trueBlocksVersion
	Settings trueBlocksSettings
}

// init sets up default values for the given configuration
func init() {
	trueBlocksViper.SetConfigName("trueBlocks")
	trueBlocksViper.SetDefault("settings.rpcProvider", "http://localhost:8545")
	trueBlocksViper.SetDefault("settings.cachePath", GetPathToRootConfig()+"cache/")
	trueBlocksViper.SetDefault("settings.indexPath", GetPathToRootConfig()+"unchained/")
	trueBlocksViper.SetDefault("settings.default_chain", "mainnet")
}

// readTrueBlocks reads and the configuration located in trueBlocks.toml file. Note
// that this routine is local to the package
func readTrueBlocks() *TrueBlocksConfig {
	if !trueBlocksRead {
		configPath := GetPathToRootConfig()
		MustReadConfig(trueBlocksViper, &cachedTrueBlocksConfig, configPath, false)

		user, _ := user.Current()

		cachePath := cachedTrueBlocksConfig.Settings.CachePath
		if len(cachePath) == 0 || cachePath == "<not_set>" {
			cachePath = path.Join(configPath, "cache") + "/"
		}
		cachePath = strings.Replace(cachePath, "$HOME", user.HomeDir, -1)
		cachePath = strings.Replace(cachePath, "~", user.HomeDir, -1)
		cachedTrueBlocksConfig.Settings.CachePath = cachePath

		indexPath := cachedTrueBlocksConfig.Settings.IndexPath
		if len(indexPath) == 0 || indexPath == "<not_set>" {
			indexPath = path.Join(configPath, "unchained") + "/"
		}
		indexPath = strings.Replace(indexPath, "$HOME", user.HomeDir, -1)
		indexPath = strings.Replace(indexPath, "~", user.HomeDir, -1)
		cachedTrueBlocksConfig.Settings.IndexPath = indexPath

		trueBlocksRead = true
	}
	return &cachedTrueBlocksConfig
}
