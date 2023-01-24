package abi

import (
	"encoding/json"
	"fmt"
	"io"
	"io/fs"
	"os"
	"path"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum/accounts/abi"
)

// Where to find know ABI files
var knownAbiSubdirectories = []string{
	"known-000", "known-005", "known-010", "known-015",
}

type AbiInterfaceMap = map[string]*types.SimpleFunction

// getMapKey turns encoding into lower case, which can be used
// as a key in AbiInterfaceMap
func getMapKey(encoding string) string {
	return strings.ToLower(encoding)
}

// LoadAbiFromJsonFile loads _standard_ JSON ABI, that is the one without encodings
// and signatures. We compute these values.
func LoadAbiFromJsonFile(filePath string, destination AbiInterfaceMap) (err error) {
	file, err := os.OpenFile(filePath, os.O_RDONLY, 0)
	if err != nil {
		return
	}

	return fromJson(file, path.Base(file.Name()), destination)
}

func fromJson(reader io.Reader, abiSource string, destination AbiInterfaceMap) (err error) {
	// Compute encodings, signatures and parse file
	loadedAbi, err := abi.JSON(reader)
	if err != nil {
		return
	}

	for _, method := range loadedAbi.Methods {
		function := types.FunctionFromAbiMethod(&method, abiSource)
		destination[getMapKey(function.Encoding)] = function
	}

	for _, ethEvent := range loadedAbi.Events {
		event := types.FunctionFromAbiEvent(&ethEvent, abiSource)
		destination[getMapKey(event.Encoding)] = event
	}

	return
}

// LoadAbiFromKnownFile loads data from _known_ ABI file, which has encodings and
// signatures.
func LoadAbiFromKnownFile(filePath string, destination AbiInterfaceMap) (err error) {
	file, err := os.OpenFile(filePath, os.O_RDONLY, 0)
	if err != nil {
		return
	}

	var functions []types.SimpleFunction
	decoder := json.NewDecoder(file)
	err = decoder.Decode(&functions)
	if err != nil {
		return
	}

	for _, function := range functions {
		destination[getMapKey(function.Encoding)] = &function
	}

	return
}

// LoadKnownAbiByName finds known ABI by name
func LoadKnownAbiByName(name string, destination AbiInterfaceMap) (err error) {
	var filePath string
	if filePath = findKnownAbi(name); filePath == "" {
		err = fmt.Errorf("known abi file not found: %s", name)
		return
	}
	return LoadAbiFromKnownFile(filePath, destination)
}

// LoadCache loads binary cache of known ABIs
func LoadCache(chain string, destination AbiInterfaceMap) (loaded bool) {
	functions, cacheErr := cache.GetAbis(chain)
	// We can ignore cache error
	if cacheErr != nil {
		return
	}

	for _, function := range functions {
		function := function
		destination[getMapKey(function.Encoding)] = &function
	}
	return true
}

func PreloadTokensAbi(destination AbiInterfaceMap) (err error) {
	err = LoadAbiFromJsonFile(
		path.Join(config.GetPathToRootConfig(), "abis", "known-000/erc_00020.json"),
		destination,
	)
	if err != nil {
		return
	}
	return LoadAbiFromJsonFile(
		path.Join(config.GetPathToRootConfig(), "abis", "known-000/erc_00721.json"),
		destination,
	)
}

// PreloadKnownAbis loads known ABI files into destination, refreshing binary cache if needed
func PreloadKnownAbis(chain string, destination AbiInterfaceMap, tokensOnly bool) (err error) {
	if tokensOnly {
		return PreloadTokensAbi(destination)
	}

	// Check if cache file is fresh
	useCache, err := cache.IsAbiCacheUpToDate(chain)
	if err != nil {
		return
	}
	// Use cache file
	if useCache {
		if loaded := LoadCache(chain, destination); loaded {
			return
		}
	}

	// Use known .json files and freshen binary cache
	paths, err := getKnownAbiPaths()
	if err != nil {
		return
	}

	for _, filePath := range paths {
		loadErr := LoadAbiFromKnownFile(filePath, destination)
		if loadErr != nil {
			return fmt.Errorf("%s: %w", filePath, loadErr)
		}
	}

	toCache := make([]types.SimpleFunction, 0, len(destination))
	for _, function := range destination {
		function := function
		toCache = append(toCache, *function)
	}
	return cache.SetAbis(chain, toCache)
}

// findKnownAbi finds known ABI by name in known-* directories
func findKnownAbi(name string) (filePath string) {
	for _, subdirName := range knownAbiSubdirectories {
		testPath := path.Join(config.GetPathToRootConfig(), "abis", subdirName, name+".json")
		if file.FileExists(testPath) {
			filePath = testPath
			return
		}
	}
	return
}

func getKnownAbiPaths() (filePaths []string, err error) {
	knownDirPath := path.Join(config.GetPathToRootConfig(), "abis")
	err = filepath.WalkDir(knownDirPath, func(path string, d fs.DirEntry, err error) error {
		if err != nil {
			return err
		}

		if d.IsDir() {
			return nil
		}
		if filepath.Ext(d.Name()) != ".json" {
			return nil
		}
		info, err := d.Info()
		if err != nil {
			return err
		}
		if info.Size() == 0 {
			return nil
		}

		filePaths = append(filePaths, path)
		return nil
	})
	return
}

// LoadAbiFromAddress loads ABI from local file or cache
func LoadAbiFromAddress(chain string, address types.Address, destination AbiInterfaceMap) (err error) {
	localFileName := address.Hex() + ".json"
	localFile, err := os.OpenFile(localFileName, os.O_RDONLY, 0)
	if os.IsNotExist(err) {
		// There's no local file, so we try to load one from cache
		loadedAbis, err := cache.GetAbi(chain, address)
		if err != nil {
			return err
		}
		for _, loadedAbi := range loadedAbis {
			loadedAbi := loadedAbi
			destination[getMapKey(loadedAbi.Encoding)] = &loadedAbi
		}
		return nil
	}
	if err != nil {
		// There was different error, we may want to report it
		return
	}
	defer localFile.Close()

	// Local file found
	if err = fromJson(localFile, path.Base(localFile.Name()), destination); err != nil {
		return
	}
	// File is correct
	if err = cache.InsertAbi(chain, address, localFile); err != nil {
		return
	}

	return
}
