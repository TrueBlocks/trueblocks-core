// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package rootConfig

import (
	"errors"
	"log"
	"os"
	"os/user"
	"path"
	"runtime"
	"strconv"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/spf13/viper"
)

// We keep a map of per-chain configs. Before reading the chain specific
// config, we read the global (top-level) config at trueBlocks.toml. Chain
// config files (if present) appear in ./configs/<chain>/<chain>.toml
// and are merged into the defaults.
var trueBlocksViper = viper.New()
var trueBlocksConfig ConfigFile

type versionGroup struct {
	Current string `toml:"current"`
}

type settingsGroup struct {
	CachePath      string `toml:"cachePath"`
	IndexPath      string `toml:"indexPath"`
	RpcProvider    string `toml:"rpcProvider"`
	DefaultChain   string `toml:"defaultChain"`
	EtherscanKey   string `toml:"etherscanKey"`
	ChainId        string `toml:"chainId"`
	PinGateway     string `toml:"pinGateway"`
	RemoteExplorer string `toml:"remoteExplorer"`
	Symbol         string `toml:"symbol"`
}

type ConfigFile struct {
	Version  versionGroup
	Settings settingsGroup
}

// init sets up default values for the given configuration
func init() {
	trueBlocksViper.SetConfigName("trueBlocks") // trueBlocks.toml (so we can find it)
	trueBlocksViper.SetDefault("Settings.RpcProvider", "http://localhost:8545")
	trueBlocksViper.SetDefault("Settings.CachePath", GetPathToRootConfig()+"cache/")
	trueBlocksViper.SetDefault("Settings.IndexPath", GetPathToRootConfig()+"unchained/")
	trueBlocksViper.SetDefault("Settings.DefaultChain", "mainnet")
	trueBlocksViper.SetDefault("Settings.EtherscanKey", "")
}

// GetRootConfig reads and the configuration located in trueBlocks.toml file. Note
// that this routine is local to the package
func GetRootConfig(chain string) *ConfigFile {
	if len(trueBlocksConfig.Settings.CachePath) == 0 {
		configPath := GetPathToRootConfig()
		MustReadConfig(trueBlocksViper, &trueBlocksConfig, configPath)

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

		if len(trueBlocksConfig.Settings.DefaultChain) == 0 {
			trueBlocksConfig.Settings.DefaultChain = "mainnet"
		}

		// Validate the URL to ensure we have it in the correct format
		// TODO: BOGUS - BREAK ONLINE TESTS
		// if !strings.HasPrefix(trueBlocksConfig.Settings.PinGateway, "http") {
		// 	logger.Fatal("missing schema in PinGateway configuration: http or https")
		// }

		// We establish only the top-level folders here. When we figure out
		// which chain we're on (not until the user tells us on the command line)
		// only then can we complete these paths. At this point these paths
		// only point to the top-levl of the cache or index. Also note that
		// these two calls do not return if they fail, so no need to handle errors
		defaultChains := []string{GetDefaultChain()}
		file.EstablishFolders(trueBlocksConfig.Settings.CachePath, defaultChains)
		file.EstablishFolders(trueBlocksConfig.Settings.IndexPath, defaultChains)
	}

	return &trueBlocksConfig
}

// GetPathToRootConfig returns the path where to find configuration files
func GetPathToRootConfig() string {
	configPath, err := PathFromXDG("XDG_CONFIG_HOME")
	if err != nil {
		log.Fatal(err)
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

	return path.Join(user.HomeDir, osPath) + "/"
}

// GetRpcProvider returns the RPC provider for a chain
func GetRpcProvider(chain string) string {
	// TODO: BOGUS-RPC PROVIDER
	return GetRootConfig(chain).Settings.RpcProvider
}

func GetDefaultChain() string {
	return GetRootConfig("mainnet").Settings.DefaultChain
}

func PathFromXDG(envVar string) (string, error) {
	// If present, we require both an existing path and a fully qualified path
	xdg := os.Getenv(envVar)
	if len(xdg) == 0 {
		return "", nil // it's okay if it's empty
	}

	if xdg[0] != '/' {
		return "", Usage("The {0} value ({1}), must be fully qualified.", envVar, xdg)
	}

	if _, err := os.Stat(xdg); err != nil {
		return "", Usage("The {0} folder ({1}) must exist.", envVar, xdg)
	}

	return path.Join(xdg, "") + "/", nil
}

// TODO: this is duplicated elsewhere because of cyclical imports - combine into usage package
func Usage(msg string, values ...string) error {
	ret := msg
	for index, val := range values {
		rep := "{" + strconv.FormatInt(int64(index), 10) + "}"
		ret = strings.Replace(ret, rep, val, -1)
	}
	return errors.New(ret)
}

// MustReadConfig calls Viper's ReadInConfig and fills values in the
// given targetStruct. Any error will result in a call to logger.Fatal
func MustReadConfig(v *viper.Viper, targetStruct interface{}, path string) {
	v.AddConfigPath(path)
	v.SetEnvPrefix("TB")
	v.AutomaticEnv()
	v.SetEnvKeyReplacer(strings.NewReplacer(".", "_"))

	err := v.ReadInConfig()
	if err != nil {
		_, ok := err.(viper.ConfigFileNotFoundError)
		// We only require some files to be present
		if !ok {
			logger.Fatal(err)
		}
	}

	err = v.Unmarshal(targetStruct)
	if err != nil {
		logger.Fatal(err)
	}
}
