// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package config

import (
	"bytes"
	"encoding/json"
	"fmt"
	"io"
	"log"
	"net/http"
	"os"
	"os/user"
	"path/filepath"
	"runtime"
	"strconv"
	"strings"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/usage"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
	"github.com/spf13/viper"
)

var trueBlocksViper = viper.New()
var trueBlocksConfig ConfigFile

type ConfigFile struct {
	Version   versionGroup          `toml:"version"`
	Settings  settingsGroup         `toml:"settings"`
	Keys      map[string]keyGroup   `toml:"keys"`
	Pinning   pinningGroup          `toml:"pinning"`
	Unchained unchainedGroup        `toml:"unchained"`
	Chains    map[string]chainGroup `toml:"chains"`
}

// init sets up default values for the given configuration
func init() {
	trueBlocksViper.SetConfigName("trueBlocks") // trueBlocks.toml (so we can find it)
	// The location of the per chain caches
	trueBlocksViper.SetDefault("Settings.CachePath", PathToRootConfig()+"cache/")
	// The location of the per chain unchained indexes
	trueBlocksViper.SetDefault("Settings.IndexPath", PathToRootConfig()+"unchained/")
	// The default chain to use if none is provided
	trueBlocksViper.SetDefault("Settings.DefaultChain", "mainnet")
	// Declare defaults for Notify so that it is read from env variables
	trueBlocksViper.SetDefault("Settings.Notify.Url", "")
	trueBlocksViper.SetDefault("Settings.Notify.Author", "")
	// The pinning gateway to query when downloading the unchained index
	trueBlocksViper.SetDefault("Pinning.GatewayUrl", defaultIpfsGateway)
	// The local endpoint for the IPFS daemon
	trueBlocksViper.SetDefault("Pinning.LocalPinUrl", "http://localhost:5001")
	// The remote endpoint for pinning on Pinata
	trueBlocksViper.SetDefault("Pinning.RemotePinUrl", "https://api.pinata.cloud/pinning/pinFileToIPFS")
	// A warning to the user not to edit the [unchained] section of the config file
	trueBlocksViper.SetDefault("Unchained.Comment", "Use this to customize the Unchained Index")
	// The default publisher of the index of none other is provided
	trueBlocksViper.SetDefault("Unchained.PreferredPublisher", "publisher.unchainedindex.eth")
	// V2: The address of the current version of the Unchained Index
	trueBlocksViper.SetDefault("Unchained.SmartContract", "0x0c316b7042b419d07d343f2f4f5bd54ff731183d")
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
		log.Fatal(err)
	}
	if err := trueBlocksViper.Unmarshal(&trueBlocksConfig); err != nil {
		log.Fatal(err)
	}

	user, _ := user.Current()

	cachePath := trueBlocksConfig.Settings.CachePath
	if len(cachePath) == 0 || cachePath == "<not_set>" {
		cachePath = filepath.Join(configPath, "cache") + "/"
	}
	cachePath = strings.Replace(cachePath, "$HOME", user.HomeDir, -1)
	cachePath = strings.Replace(cachePath, "~", user.HomeDir, -1)
	if !strings.Contains(cachePath, "/cache") {
		cachePath = filepath.Join(cachePath, "cache")
	}
	trueBlocksConfig.Settings.CachePath = cachePath

	indexPath := trueBlocksConfig.Settings.IndexPath
	if len(indexPath) == 0 || indexPath == "<not_set>" {
		indexPath = filepath.Join(configPath, "unchained") + "/"
	}
	indexPath = strings.Replace(indexPath, "$HOME", user.HomeDir, -1)
	indexPath = strings.Replace(indexPath, "~", user.HomeDir, -1)
	if !strings.Contains(indexPath, "/unchained") {
		indexPath = filepath.Join(indexPath, "unchained")
	}
	trueBlocksConfig.Settings.IndexPath = indexPath

	if len(trueBlocksConfig.Settings.DefaultChain) == 0 {
		trueBlocksConfig.Settings.DefaultChain = "mainnet"
	}

	// migrate the config file if necessary (note that this does not return if the file is migrated).
	currentVer := version.NewVersion(trueBlocksConfig.Version.Current)
	_ = migrate(currentVer)

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
		isDefaulted := len(ch.IpfsGateway) == 0 || strings.Trim(ch.IpfsGateway, "/") == strings.Trim(defaultIpfsGateway, "/")
		if isDefaulted {
			ch.IpfsGateway = trueBlocksConfig.Pinning.GatewayUrl
		}
		ch.IpfsGateway = strings.Replace(ch.IpfsGateway, "[{CHAIN}]", "ipfs", -1)
		ch.LocalExplorer = clean(ch.LocalExplorer)
		ch.RemoteExplorer = clean(ch.RemoteExplorer)
		ch.RpcProvider = strings.Trim(clean(ch.RpcProvider), "/") // Infura, for example, doesn't like the trailing slash
		if err := validateRpcEndpoint(ch.Chain, ch.RpcProvider); err != nil {
			logger.Fatal(err)
		}
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

// PathToConfigFile returns the path where to find the configuration file
func PathToConfigFile() string {
	configFolder := PathToRootConfig()
	return filepath.Join(configFolder, "trueBlocks.toml")
}

// PathToRootConfig returns the path where to find configuration files
func PathToRootConfig() string {
	configPath, err := pathFromXDG("XDG_CONFIG_HOME")
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

func validateRpcEndpoint(chain, provider string) error {
	if utils.IsPermitted() {
		return nil
	}

	if provider == "https:" {
		problem := `No rpcProvider found.`
		return usage.Usage(rpcWarning, chain, provider, problem)
	}

	if !strings.HasPrefix(provider, "http") {
		problem := `Invalid rpcProvider found (must be a url).`
		return usage.Usage(rpcWarning, chain, provider, problem)
	}

	if chain == "mainnet" {
		// TODO: Eventually this will be parameterized, for example, when we start publishing to Optimism
		deployed := uint64(14957097) // block where the unchained index was deployed to mainnet
		if err := checkUnchainedProvider(chain, deployed); err != nil {
			return err
		}
	}

	return nil
}

func checkUnchainedProvider(chain string, deployed uint64) error {
	// TODO: Clean this up
	// TODO: We need to check that the unchained index has been deployed on the chain
	if os.Getenv("TB_NO_PROVIDER_CHECK") == "true" {
		logger.Info("Skipping rpcProvider check")
		return nil
	}
	url := trueBlocksViper.Get("chains." + chain + ".rpcProvider").(string)
	str := `{ "jsonrpc": "2.0", "method": "eth_getBlockByNumber", "params": [ "{0}", true ], "id": 1 }`
	payLoad := []byte(strings.Replace(str, "{0}", fmt.Sprintf("0x%x", deployed), -1))
	req, err := http.NewRequest("POST", url, bytes.NewBuffer(payLoad))
	if err != nil {
		return fmt.Errorf("error creating request to rpcProvider (%s): %v", url, err)
	}
	req.Header.Set("Content-Type", "application/json")
	client := &http.Client{}
	resp, err := client.Do(req)
	if err != nil {
		return fmt.Errorf("error making request to rpcProvider (%s): %v", url, err)
	}
	defer resp.Body.Close()
	if resp.StatusCode != http.StatusOK {
		return fmt.Errorf("unexpected status code from rpcProvider (%s): %d, expected: %d", url, resp.StatusCode, http.StatusOK)
	}
	body, err := io.ReadAll(resp.Body)
	if err != nil {
		return fmt.Errorf("error reading response body: %v", err)
	}
	var result map[string]interface{}
	if err := json.Unmarshal(body, &result); err != nil {
		return fmt.Errorf("error unmarshalling response: %v", err)
	}
	s := result["result"].(map[string]interface{})["number"].(string)
	if bn, _ := strconv.ParseUint(s, 0, 64); bn != deployed {
		msg := `the unchained index was deployed at block %d. Is your node synced that far?`
		return fmt.Errorf(msg, deployed)
	}
	return nil
}

var rpcWarning string = `
We found a problem with the rpcProvider for the {0} chain.

	Provider: {1}
	Chain:    {0}
	Problem:  {2}

Confirm the value for the given provider. You may edit this value with
"chifra config edit".

Also, try the following curl command. If this command does not work, neither will chifra.

curl -X POST -H "Content-Type: application/json" --data '{ "jsonrpc": "2.0", "method": "web3_clientVersion", "id": 6 }' {1}
`
