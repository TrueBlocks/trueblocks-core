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

var trueBlocksViper = viper.New()
var trueBlocksRead = false
var cachedTrueBlocksConfig TrueBlocksConfig

type trueBlocksVersion struct {
	Current string
}

type trueBlocksSettings struct {
	RpcProvider  string
	CachePath    string
	IndexPath    string
	EtherscanKey string `toml:"etherscan_key"`
}

type TrueBlocksConfig struct {
	Version  trueBlocksVersion
	Settings trueBlocksSettings
}

// init sets up default values for the given configuration
func init() {
	trueBlocksViper.SetConfigName("trueBlocks")
	trueBlocksViper.SetDefault("Settings.RpcProvider", "http://localhost:8545")
	trueBlocksViper.SetDefault("Settings.CachePath", GetPathToRootConfig()+"cache/")
	trueBlocksViper.SetDefault("Settings.IndexPath", GetPathToRootConfig()+"unchained/")
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

		// We establish only the top-level folders here. When we figure out
		// which chain we're on (not until the user tells us on the command line)
		// only then can we complete these paths. At this point these paths
		// only point to the top-levl of the cache or index. Also note that
		// these two calls do not return if they fail, so no need to handle errors
		// TODO: BOGUS-DEFAULTCHAIN
		defaultChains := []string{GetDefaultChain()}
		file.EstablishFolders(cachedTrueBlocksConfig.Settings.CachePath, defaultChains)
		file.EstablishFolders(cachedTrueBlocksConfig.Settings.IndexPath, defaultChains)

		trueBlocksRead = true
	}

	return &cachedTrueBlocksConfig
}

func GetDefaultChain() string {
	// TODO: BOGUS-DEFAULTCHAIN
	return "mainnet"
}

func GetTestChain() string {
	// TODO: BOGUS-DEFAULTCHAIN
	return "mainnet"
}
