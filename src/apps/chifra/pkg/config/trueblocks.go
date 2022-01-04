// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package config

import (
	"os/user"
	"path"
	"runtime"

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
	trueBlocksViper.SetDefault("Settings.CachePath", GetPathToConfig("cache"))
	trueBlocksViper.SetDefault("Settings.IndexPath", GetPathToConfig("unchained"))
	trueBlocksViper.SetDefault("Settings.Chain", "mainnet")
}

// ReadGlobal reads and the configuration located in trueBlocks.toml file
func ReadTrueBlocks() *TrueBlocksConfig {
	if !trueBlocksRead {
		MustReadConfig(trueBlocksViper, &cachedTrueBlocksConfig, GetPathToConfig(""), false)
		trueBlocksRead = true
	}

	return &cachedTrueBlocksConfig
}

var OsToPath = map[string]string{
	"linux":  ".local/share/trueblocks",
	"darwin": "Library/Application Support/TrueBlocks",
}

// TODO: Search for PathAccessor
// GetPathToConfig returns the path to the directory where the configuration files are
func GetPathToConfig(fileName string) string {
	// These values are checked in CheckMigrations and will not proceed if not valid
	userOs := runtime.GOOS
	user, _ := user.Current()
	return path.Join(user.HomeDir, OsToPath[userOs], fileName)
}

// TODO: Search for PathAccessor
// GetPathToCache returns the one and only cachePath
func GetPathToCache() string {
	return ReadTrueBlocks().Settings.CachePath
}

// TODO: Search for PathAccessor
// GetPathToIndex returns the one and only cachePath
func GetPathToIndex() string {
	return ReadTrueBlocks().Settings.IndexPath
}

// TODO: Search for PathAccessor
// GetPathToCommands returns full path the the given tool
func GetPathToCommands(part string) string {
	usr, _ := user.Current()
	dir := usr.HomeDir
	return dir + "/.local/bin/chifra/" + part
}
