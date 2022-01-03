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
	trueBlocksViper.SetDefault("Settings.CachePath", GetPathToConfig("cache"))
	trueBlocksViper.SetDefault("Settings.IndexPath", GetPathToConfig("unchained"))
}

// ReadGlobal reads and the configuration located in trueBlocks.toml file
func ReadTrueBlocks() *TrueBlocksConfig {
	if !trueBlocksRead {
		MustReadConfig(trueBlocksViper, &cachedTrueBlocksConfig, false)
		l := len(cachedTrueBlocksConfig.Settings.CachePath)
		if l > 0 {
			if cachedTrueBlocksConfig.Settings.CachePath[l-1] != '/' {
				cachedTrueBlocksConfig.Settings.CachePath = cachedTrueBlocksConfig.Settings.CachePath + "/"
			}
		}
		l = len(cachedTrueBlocksConfig.Settings.IndexPath)
		if l > 0 {
			if cachedTrueBlocksConfig.Settings.IndexPath[l-1] != '/' {
				cachedTrueBlocksConfig.Settings.IndexPath = cachedTrueBlocksConfig.Settings.IndexPath + "/"
			}
		}
		trueBlocksRead = true
	}

	return &cachedTrueBlocksConfig
}
