package abi

import (
	"encoding/json"
	"fmt"
	"io/fs"
	"log"
	"os"
	"path"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum/accounts/abi"
	"github.com/ethereum/go-ethereum/common"
)

var knownAbiSubdirectories = []string{
	"known-000", "known-005", "known-010", "known-015",
}

// !!!
// https://pkg.go.dev/github.com/ethereum/go-ethereum/accounts/abi#pkg-overview

type AbiInterfaceMap = map[string]*types.SimpleFunction

func getMapKey(encoding string) string {
	return strings.ToLower(encoding)
}

func LoadAbiFromJsonFile(filePath string, destination AbiInterfaceMap) (err error) {
	file, err := os.OpenFile(filePath, os.O_RDONLY, 0)
	if err != nil {
		return
	}

	loadedAbi, err := abi.JSON(file)
	if err != nil {
		return
	}

	for _, method := range loadedAbi.Methods {
		// method.ID is our "four-byte"
		fourByte := string(common.Bytes2Hex(method.ID))
		log.Println("Read", fourByte, method.Name)

		var functionType string
		switch method.Type {
		case abi.Constructor:
			functionType = "constructor"
		case abi.Fallback:
			functionType = "fallback"
		case abi.Receive:
			functionType = "receive"
		default:
			functionType = "function"
		}
		destination[fourByte] = &types.SimpleFunction{
			Encoding:        fourByte,
			Signature:       method.Sig,
			Name:            method.Name,
			AbiSource:       path.Base(file.Name()),
			FunctionType:    functionType,
			Constant:        method.Constant,
			StateMutability: method.StateMutability,
			// TODO:
			// Inputs:
			// Outputs:
		}
	}

	for _, event := range loadedAbi.Events {
		// method.ID is our "four-byte"
		fourByte := event.ID.Hex()
		log.Println("Read event", fourByte, event.Name)
		destination[fourByte] = &types.SimpleFunction{
			Encoding:     fourByte,
			Signature:    event.Sig,
			Name:         event.Name,
			AbiSource:    path.Base(file.Name()),
			FunctionType: "event",
			Anonymous:    event.Anonymous,
			// TODO:
			// Inputs:
			// Outputs:
		}
	}

	return
}

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
		destination[function.Encoding] = &function
	}

	return
}

func LoadKnownAbiByName(name string, destination AbiInterfaceMap) (err error) {
	var filePath string
	if filePath = findKnownAbi(name); filePath == "" {
		err = fmt.Errorf("known abi file not found: %s", name)
		return
	}
	return LoadAbiFromJsonFile(filePath, destination)
}

func LoadCache(chain string, destination AbiInterfaceMap) (loaded bool) {
	functions, cacheErr := cache.GetAbis(chain)
	// We can ignore cache error
	if cacheErr != nil {
		log.Println("error while loading from cache:", cacheErr)
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

func PreloadKnownAbis(chain string, destination AbiInterfaceMap, tokensOnly bool) (err error) {
	if tokensOnly {
		return PreloadTokensAbi(destination)
	}

	// Check if cache file is fresh
	useCache, err := cache.IsAbiCacheUpToDate(chain)
	if err != nil {
		return
	}
	log.Println("ABI cache up-to-date", useCache)

	// Use cache file
	if useCache {
		loaded := LoadCache(chain, destination)
		if !loaded {
			log.Println("Could not load binary cache")
		} else {
			log.Println("Loaded binary cache")
		}
		// TODO: only return if cache was loaded
		return
	}

	// Use known .json files and freshen binary cache
	paths, err := getKnownAbiPaths()
	if err != nil {
		return
	}
	fileCount := 0

	for _, filePath := range paths {
		loadErr := LoadAbiFromKnownFile(filePath, destination)
		if loadErr != nil {
			return fmt.Errorf("%s: %w", filePath, loadErr)
		}
		fileCount++
	}

	log.Println("Loaded", len(destination), "interfaces from", fileCount, "JSON files")
	log.Println("Freshening abi cache")

	toCache := make([]types.SimpleFunction, 0, len(destination))
	for _, function := range destination {
		function := function
		toCache = append(toCache, *function)
	}
	err = cache.SetAbis(chain, toCache)
	if err == nil {
		log.Printf("Saved %d interfaces in binary cache", len(toCache))
	}

	return
}

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

func LoadAbiFromAddress(address common.Address) (err error) {
	if

	return
}
