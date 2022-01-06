// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package config

import (
	"os/user"
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
	Chain        string
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
	trueBlocksViper.SetDefault("Settings.CachePath", GetPathToConfig(false /* withChain */)+"cache")
	trueBlocksViper.SetDefault("Settings.IndexPath", GetPathToConfig(false /* withChain */)+"unchained")
	trueBlocksViper.SetDefault("Settings.Chain", "mainnet")
}

// ReadGlobal reads and the configuration located in trueBlocks.toml file
func ReadTrueBlocks() *TrueBlocksConfig {
	if !trueBlocksRead {
		path := GetPathToConfig(false /* withChain */)
		MustReadConfig(trueBlocksViper, &cachedTrueBlocksConfig, path, false)

		user, _ := user.Current()

		cachePath := cachedTrueBlocksConfig.Settings.CachePath
		cachePath = strings.Replace(cachePath, "$HOME", user.HomeDir, -1)
		cachePath = strings.Replace(cachePath, "~", user.HomeDir, -1)
		cachedTrueBlocksConfig.Settings.CachePath = cachePath

		indexPath := cachedTrueBlocksConfig.Settings.IndexPath
		indexPath = strings.Replace(indexPath, "$HOME", user.HomeDir, -1)
		indexPath = strings.Replace(indexPath, "~", user.HomeDir, -1)
		cachedTrueBlocksConfig.Settings.IndexPath = indexPath

		// We want to establish that these two folders exist. Note, however,
		// that these two paths are just the base paths. When we actually
		// query a value, we will append 'chain'. The trouble is we don't know
		// chain until the command line is parsed. Also note that these two
		// routines do not return if they fail
		var none []string
		file.EstablishFolders(cachedTrueBlocksConfig.Settings.CachePath, none /* folders */)
		file.EstablishFolders(cachedTrueBlocksConfig.Settings.IndexPath, none /* folders */)

		trueBlocksRead = true
	}

	return &cachedTrueBlocksConfig
}
