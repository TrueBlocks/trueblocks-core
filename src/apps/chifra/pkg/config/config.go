// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package config

import (
	"os"
	"os/user"
	"path/filepath"
	"runtime"
	"strings"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/usage"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
	"github.com/spf13/viper"
)

var trueBlocksViper = viper.New()
var trueBlocksConfig ConfigFile

type ConfigFile struct {
	Version  versionGroup          `toml:"version"`
	Settings settingsGroup         `toml:"settings"`
	Keys     map[string]keyGroup   `toml:"keys"`
	Chains   map[string]chainGroup `toml:"chains"`
}

// init sets up default values for the given configuration
func init() {
	trueBlocksViper.SetConfigName("trueBlocks") // trueBlocks.toml (so we can find it)
	trueBlocksViper.SetDefault("Settings.CachePath", PathToRootConfig()+"cache/")
	trueBlocksViper.SetDefault("Settings.IndexPath", PathToRootConfig()+"unchained/")
	trueBlocksViper.SetDefault("Settings.DefaultChain", "mainnet")
	trueBlocksViper.SetDefault("Settings.DefaultGateway", "https://ipfs.unchainedindex.io/ipfs")
	trueBlocksViper.SetDefault("Settings.LocalGateway", "http://localhost:5001")
}

var configMutex sync.Mutex
var configLoaded = false

// GetRootConfig reads and the configuration located in trueBlocks.toml file. Note
// that this routine is local to the package
func GetRootConfig() *ConfigFile {
	if configLoaded {
		return &trueBlocksConfig
	}
	configMutex.Lock()
	defer configMutex.Unlock()

	configPath := PathToRootConfig()
	trueBlocksViper.AddConfigPath(configPath)
	trueBlocksViper.SetEnvPrefix("TB")
	trueBlocksViper.AutomaticEnv()
	trueBlocksViper.SetEnvKeyReplacer(strings.NewReplacer(".", "_"))
	if err := trueBlocksViper.ReadInConfig(); err != nil {
		logger.Fatal(err)
	}
	if err := trueBlocksViper.Unmarshal(&trueBlocksConfig); err != nil {
		logger.Fatal(err)
	}

	requiredVer := version.NewVersion("v1.0.0-release")
	currentVer := version.NewVersion(trueBlocksConfig.Version.Current)
	if currentVer.Uint64() < requiredVer.Uint64() {
		_ = UpgradeConfigs(requiredVer) // does not return
	}

	user, _ := user.Current()

	cachePath := trueBlocksConfig.Settings.CachePath
	if len(cachePath) == 0 || cachePath == "<not_set>" {
		cachePath = filepath.Join(configPath, "cache") + "/"
	}
	cachePath = strings.Replace(cachePath, "$HOME", user.HomeDir, -1)
	cachePath = strings.Replace(cachePath, "~", user.HomeDir, -1)
	trueBlocksConfig.Settings.CachePath = cachePath

	indexPath := trueBlocksConfig.Settings.IndexPath
	if len(indexPath) == 0 || indexPath == "<not_set>" {
		indexPath = filepath.Join(configPath, "unchained") + "/"
	}
	indexPath = strings.Replace(indexPath, "$HOME", user.HomeDir, -1)
	indexPath = strings.Replace(indexPath, "~", user.HomeDir, -1)
	trueBlocksConfig.Settings.IndexPath = indexPath

	if len(trueBlocksConfig.Settings.DefaultChain) == 0 {
		trueBlocksConfig.Settings.DefaultChain = "mainnet"
	}

	// We establish only the top-level folders here. When we figure out
	// which chain we're on (not until the user tells us on the command line)
	// only then can we complete these paths. At this point these paths
	// only point to the top-levl of the cache or index. Also note that
	// these two calls do not return if they fail, so no need to handle errors
	defaultChains := []string{trueBlocksConfig.Settings.DefaultChain}
	_ = file.EstablishFolders(trueBlocksConfig.Settings.CachePath, defaultChains)
	_ = file.EstablishFolders(trueBlocksConfig.Settings.IndexPath, defaultChains)

	// clean up the config data
	for chain, ch := range trueBlocksConfig.Chains {
		clean := func(url string) string {
			if !strings.HasPrefix(url, "http") {
				url = "https://" + url
			}
			if !strings.HasSuffix(url, "/") {
				url += "/"
			}
			return url
		}
		ch.Chain = chain
		if len(ch.IpfsGateway) == 0 {
			ch.IpfsGateway = clean(trueBlocksConfig.Settings.DefaultGateway)
		}
		ch.LocalExplorer = clean(ch.LocalExplorer)
		ch.RemoteExplorer = clean(ch.RemoteExplorer)
		ch.RpcProvider = clean(ch.RpcProvider)
		ch.IpfsGateway = clean(ch.IpfsGateway)
		if ch.Scrape.AppsPerChunk == 0 {
			settings := ScrapeSettings{
				AppsPerChunk: 2000000,
				SnapToGrid:   250000,
				FirstSnap:    2000000,
				UnripeDist:   28,
				ChannelCount: 20,
				AllowMissing: false,
			}
			if chain == "mainnet" {
				settings.SnapToGrid = 100000
				settings.FirstSnap = 2300000
			}
			ch.Scrape = settings
		}
		trueBlocksConfig.Chains[chain] = ch
	}
	configLoaded = true
	return &trueBlocksConfig
}

// PathToRootConfig returns the path where to find configuration files
func PathToRootConfig() string {
	configPath, err := pathFromXDG("XDG_CONFIG_HOME")
	if err != nil {
		logger.Fatal(err)
	} else if len(configPath) > 0 {
		return configPath
	}

	// The migration code will have already checked for invalid operating systems (i.e. Windows)
	userOs := runtime.GOOS
	if len(os.Getenv("TEST_OS")) > 0 {
		userOs = os.Getenv("TEST_OS")
	}

	user, _ := user.Current()
	osPath := ".local/share/trueblocks"
	if userOs == "darwin" {
		osPath = "Library/Application Support/TrueBlocks"
	}

	return filepath.Join(user.HomeDir, osPath) + "/"
}

func pathFromXDG(envVar string) (string, error) {
	// If present, we require both an existing path and a fully qualified path
	xdg := os.Getenv(envVar)
	if len(xdg) == 0 {
		return "", nil // it's okay if it's empty
	}

	if xdg[0] != '/' {
		return "", usage.Usage("The {0} value ({1}), must be fully qualified.", envVar, xdg)
	}

	if _, err := os.Stat(xdg); err != nil {
		return "", usage.Usage("The {0} folder ({1}) must exist.", envVar, xdg)
	}

	return filepath.Join(xdg, "") + "/", nil
}
