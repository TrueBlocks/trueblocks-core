// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package config

import (
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
	trueBlocksViper.SetDefault("Settings.CachePath", GetPathToConfig(true /* withChain */)+"cache")
	trueBlocksViper.SetDefault("Settings.IndexPath", GetPathToConfig(true /* withChain */)+"unchained")
	trueBlocksViper.SetDefault("Settings.Chain", "mainnet")
}

// ReadGlobal reads and the configuration located in trueBlocks.toml file
func ReadTrueBlocks() *TrueBlocksConfig {
	if !trueBlocksRead {
		MustReadConfig(trueBlocksViper, &cachedTrueBlocksConfig, GetPathToConfig(false /* withChain */), false)
		trueBlocksRead = true
	}

	return &cachedTrueBlocksConfig
}
