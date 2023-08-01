package abi

import (
	"errors"
	"fmt"
	"io"
	"io/fs"
	"os"
	"path"
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum/accounts/abi"
)

// Where to find know ABI files
var knownAbiSubdirectories = []string{
	"known-000", "known-005", "known-010", "known-015",
}

type AbiInterfaceMap = map[string]*types.SimpleFunction

// LoadAbiFromJsonFile loads _standard_ JSON ABI, that is the one without encodings
// and signatures. We compute these values.
func LoadAbiFromJsonFile(filePath string, destination AbiInterfaceMap) (err error) {
	file, err := os.OpenFile(filePath, os.O_RDONLY, 0)
	if err != nil {
		return
	}

	return fromJson(file, destination)
}

func fromJson(reader io.Reader, destination AbiInterfaceMap) (err error) {
	// Compute encodings, signatures and parse file
	loadedAbi, err := abi.JSON(reader)
	if err != nil {
		return
	}

	for _, method := range loadedAbi.Methods {
		method := method
		function := types.FunctionFromAbiMethod(&method)
		destination[function.Encoding] = function
	}

	for _, ethEvent := range loadedAbi.Events {
		ethEvent := ethEvent
		event := types.FunctionFromAbiEvent(&ethEvent)
		destination[event.Encoding] = event
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

	// We still need abi.Method and abi.Event, so will just use fromJson
	return fromJson(file, destination)
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
		function.Normalize()
		destination[function.Encoding] = &function
	}
	return true
}

// PreloadKnownAbis loads known ABI files into destination, refreshing binary cache if needed
func PreloadKnownAbis(chain string, destination AbiInterfaceMap) (err error) {
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
func LoadAbiFromAddress(chain string, address base.Address, destination AbiInterfaceMap) (err error) {
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
			loadedAbi.Normalize()
			destination[loadedAbi.Encoding] = &loadedAbi
		}
		return nil
	}
	if err != nil {
		// There was different error, we may want to report it
		return
	}
	defer localFile.Close()

	// Local file found
	if err = fromJson(localFile, destination); err != nil {
		return
	}
	// File is correct
	if err = cache.InsertAbi(chain, address, localFile); err != nil {
		return
	}

	return
}

// LoadAbi tries to load ABI from any source (local file, cache, download from 3rd party)
func LoadAbi(chain string, address base.Address, destination AbiInterfaceMap) (err error) {
	if err = PreloadKnownAbis(chain, destination); err != nil {
		return
	}

	// If there was no error, the abi was loaded...
	err = LoadAbiFromAddress(chain, address, destination)
	if err == nil {
		return
	}

	// If there was an unexpected error (not NotExist or not an empty file), return the error
	if !os.IsNotExist(err) && err != io.EOF {
		return fmt.Errorf("while reading %s ABI file: %w", address, err)
	}

	// We didn't find the file. Check if the address is a contract
	conn := rpcClient.NewConnection(chain, []string{})
	if err := conn.IsContractAt(chain, address, nil); err != nil && !errors.Is(err, rpcClient.ErrNotAContract) {
		return err
	} else if errors.Is(err, rpcClient.ErrNotAContract) {
		return nil
	}

	// Fetch ABI from a provider
	return DownloadAbi(chain, address, destination)
}
