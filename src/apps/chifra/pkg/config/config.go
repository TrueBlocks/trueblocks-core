package config

import (
	"bytes"
	"encoding/json"
	"fmt"
	"io"
	"net/http"
	"os"
	"os/user"
	"path/filepath"
	"runtime"
	"strconv"
	"strings"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/configtypes"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/usage"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
)

const envPrefix = "TB_"

var trueBlocksConfig configtypes.Config
var cachePath string
var indexPath string

// init sets up default values for the given configuration
func init() {
	// The location of the per chain caches
	cachePath = filepath.Join(PathToRootConfig(), "cache")
	// The location of the per chain unchained indexes
	indexPath = filepath.Join(PathToRootConfig(), "unchained")
}

var configMutex sync.Mutex
var configLoaded = false

func loadFromTomlFile(filePath string, dest *configtypes.Config) error {
	if err := ReadToml(filePath, dest); err != nil {
		return fmt.Errorf("error reading config file %s: %w", filePath, err)
	} else {
		fileVer := version.NewVersion(dest.Version.Current)
		curVers := version.NewVersion(version.LibraryVersion)
		if fileVer.Uint64() >= curVers.Uint64() {
			return nil
		}

		// As of version 5.0.0, we spin through the config.Chains map and if a
		// chain's rpcProviders array is empty but its rpcProvider string is
		// not, append the rpcProvider string to the rpcProviders array and
		// then empty out the rpcProvider string.
		changed := false
		for chain, ch := range dest.Chains {
			if len(ch.RpcProviders) == 0 && len(ch.GetRpcProvider()) > 0 {
				ch.RpcProviders = []string{ch.GetRpcProvider()}
				ch.RpcProviderOld = ""
				changed = true
			}
			dest.Chains[chain] = ch
		}
		if changed {
			if err := dest.WriteFile(filePath, curVers); err != nil {
				return fmt.Errorf("error writing config file %s: %w", filePath, err)
			}
		}
		return nil
	}
}

// GetRootConfig reads and the configuration located in trueBlocks.toml file. Note
// that this routine is local to the package
func GetRootConfig() *configtypes.Config {
	if configLoaded {
		return &trueBlocksConfig
	}
	configMutex.Lock()
	defer configMutex.Unlock()

	configPath := PathToRootConfig()

	// First load the default config
	trueBlocksConfig = configtypes.NewConfig(cachePath, indexPath, defaultIpfsGateway)

	// Load TOML file
	tomlConfigFn := filepath.Join(configPath, "trueBlocks.toml")
	if err := loadFromTomlFile(tomlConfigFn, &trueBlocksConfig); err != nil {
		logger.Panic("loading config from .toml file:", err)
	}

	// Load ENV variables
	if err := loadFromEnv(envPrefix, &trueBlocksConfig); err != nil {
		logger.Panic("loading config from environment variables:", err)
	}

	user, _ := user.Current()

	cachePath := trueBlocksConfig.Settings.CachePath
	cachePath = strings.ReplaceAll(cachePath, "/", string(os.PathSeparator))
	if len(cachePath) == 0 || cachePath == "<not_set>" {
		cachePath = filepath.Join(configPath, "cache")
	}
	cachePath = strings.ReplaceAll(cachePath, "$HOME", user.HomeDir)
	cachePath = strings.ReplaceAll(cachePath, "~", user.HomeDir)
	if filepath.Base(cachePath) != "cache" {
		cachePath = filepath.Join(cachePath, "cache")
	}
	trueBlocksConfig.Settings.CachePath = cachePath

	indexPath := trueBlocksConfig.Settings.IndexPath
	indexPath = strings.ReplaceAll(indexPath, "/", string(os.PathSeparator))
	if len(indexPath) == 0 || indexPath == "<not_set>" {
		indexPath = filepath.Join(configPath, "unchained")
	}
	indexPath = strings.ReplaceAll(indexPath, "$HOME", user.HomeDir)
	indexPath = strings.ReplaceAll(indexPath, "~", user.HomeDir)
	if filepath.Base(indexPath) != "unchained" {
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
		ch.IpfsGateway = strings.ReplaceAll(ch.IpfsGateway, "[{CHAIN}]", "ipfs")
		ch.LocalExplorer = clean(ch.LocalExplorer)
		ch.RemoteExplorer = clean(ch.RemoteExplorer)
		rpc := strings.Trim(clean(ch.GetRpcProvider()), "/") // Infura, for example, doesn't like the trailing slash
		ch.RpcProviders = append(ch.RpcProviders, rpc)
		if err := validateRpcEndpoint(ch.Chain, ch.GetRpcProvider()); err != nil {
			logger.Panic(err)
		}
		ch.IpfsGateway = clean(ch.IpfsGateway)
		if ch.Scrape.AppsPerChunk == 0 {
			settings := configtypes.ScrapeSettings{
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

// ReloadConfig clears the config cache so that the next time ReadToml is
// called the config will reload
func ReloadConfig() {
	configLoaded = false
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
		logger.Panic(err)
	} else if len(configPath) > 0 {
		return configPath
	}

	// The migration code will have already checked for invalid operating systems
	userOs := runtime.GOOS
	if len(os.Getenv("TEST_OS")) > 0 {
		userOs = os.Getenv("TEST_OS")
	}

	user, _ := user.Current()
	osPath := ".local/share/trueblocks"
	switch userOs {
	case "darwin":
		osPath = "Library/Application Support/TrueBlocks"
	case "windows":
		osPath = "AppData/Local/trueblocks"
	}

	return filepath.Join(user.HomeDir, osPath)
}

func pathFromXDG(envVar string) (string, error) {
	// If present, we require both an existing path and a fully qualified path
	xdg := os.Getenv(envVar)
	if len(xdg) == 0 {
		return "", nil // it's okay if it's empty
	}

	absXDGPath, err := filepath.Abs(xdg)
	if err != nil {
		return "", usage.Usage("The {0} value ({1}), could not be interpreted as an absolute path.", envVar, xdg)
	}

	if _, err := os.Stat(absXDGPath); err != nil {
		return "", usage.Usage("The {0} folder ({1}) must exist.", envVar, absXDGPath)
	}

	return filepath.Join(absXDGPath, ""), nil
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
	url := trueBlocksConfig.Chains[chain].GetRpcProvider()
	str := `{ "jsonrpc": "2.0", "method": "eth_getBlockByNumber", "params": [ "{0}", true ], "id": 1 }`
	payLoad := []byte(strings.ReplaceAll(str, "{0}", fmt.Sprintf("0x%x", deployed)))
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
