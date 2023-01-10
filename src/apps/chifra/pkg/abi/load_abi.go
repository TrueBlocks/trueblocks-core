package abi

import (
	"errors"
	"fmt"
	"log"
	"os"
	"path"
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

var errInvalidJson = errors.New("parsing JSON failed")

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
		destination[fourByte] = &types.SimpleFunction{
			Encoding:     fourByte,
			Signature:    method.Sig,
			Name:         method.Name,
			FunctionType: "function",
			AbiSource:    file.Name(),
			// TODO: check if this is correct
			Anonymous:       method.Name != "",
			Constant:        method.Constant,
			StateMutability: method.StateMutability,
			// TODO:
			// Inputs:
			// Outputs:
		}
	}

	return
}

func LoadAbiFromKnownFile(name string, destination AbiInterfaceMap) (err error) {
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
	err = LoadAbiFromJsonFile(
		path.Join(config.GetPathToRootConfig(), "abis", "known-000/erc_00721.json"),
		destination,
	)
	if err != nil {
		return
	}
	// mergeMaps(destination, erc721)
	return
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
		return
	}

	// Use cache .json files and freshen binary cache
	cachePath := path.Join(
		config.GetPathToCache(chain),
		"abis",
	)
	files, err := os.ReadDir(cachePath)
	if err != nil {
		return
	}
	fileCount := 0

	for _, file := range files {
		if file.IsDir() {
			continue
		}
		if path.Ext(file.Name()) != ".json" {
			continue
		}
		var info os.FileInfo
		info, err = file.Info()
		if err != nil {
			return
		}
		if info.Size() == 0 {
			continue
		}

		filePath := path.Join(
			cachePath,
			file.Name(),
		)
		loadErr := LoadAbiFromJsonFile(filePath, destination)
		if loadErr != nil && errors.Is(loadErr, errInvalidJson) {
			log.Println("deleting invalid JSON file:", file.Name())
			_ = os.Remove(filePath)
			continue
		}
		if loadErr != nil {
			return fmt.Errorf("%s: %w", file.Name(), loadErr)
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
