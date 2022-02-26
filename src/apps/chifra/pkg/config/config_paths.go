// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package config

import (
	"log"
	"os"
	"os/user"
	"path"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

const chainConfigMustExist string = `

	The chain configuration folder for chain {0} does not exist. Is it correct?
	Check: {1}

`

// GetPathToChainConfig returns the chain-specific config folder
func GetPathToChainConfig(chain string) string {
	// We always need a chain
	if len(chain) == 0 {
		chain = GetDefaultChain()
	}
	ret := GetPathToRootConfig()

	// Our configuration files are always in ./config folder relative to top most folder
	cfgFolder := path.Join(ret, "config/", chain) + "/"
	if _, err := os.Stat(cfgFolder); err != nil {
		log.Fatal(Usage(chainConfigMustExist, chain, cfgFolder))
	}
	return cfgFolder
}

// GetPathToIndex returns the one and only cachePath
func GetPathToIndex(chain string) string {
	// We need the index path from either XDG which dominates or the config file
	indexPath, err := PathFromXDG("XDG_CACHE_HOME")
	if err != nil {
		log.Fatal(err)
	} else if len(indexPath) == 0 {
		indexPath = GetRootConfig().Settings.IndexPath
	}

	// We want the index folder to be named `unchained` and be in
	// the root of cache path
	if !strings.Contains(indexPath, "/unchained") {
		indexPath = path.Join(indexPath, "unchained")
	}

	// We always have to have a chain...
	if len(chain) == 0 {
		chain = GetDefaultChain()
	}

	// We know what we want, create it if it doesn't exist and return it
	newPath := path.Join(indexPath, chain) + "/"
	EstablishIndexPaths(newPath)
	return newPath
}

// GetPathToCache returns the one and only cachePath
func GetPathToCache(chain string) string {
	// We need the index path from either XDG which dominates or the config file
	cachePath, err := PathFromXDG("XDG_CACHE_HOME")
	if err != nil {
		log.Fatal(err)
	} else if len(cachePath) == 0 {
		cachePath = GetRootConfig().Settings.CachePath
	}

	// We want the cache folder to be named `cache` and be in
	// the root of cache path
	if !strings.Contains(cachePath, "/cache") {
		cachePath = path.Join(cachePath, "cache")
	}

	// We always have to have a chain...
	if len(chain) == 0 {
		chain = GetDefaultChain()
	}

	// We know what we want, create it if it doesn't exist and return it
	newPath := path.Join(cachePath, chain) + "/"
	EstablishCachePaths(newPath)
	return newPath
}

func GetTestChain() string {
	// This does not get customized per chain. We can only test against mainnet currently
	return "mainnet"
}

// GetPathToCommands returns full path the the given tool
func GetPathToCommands(part string) string {
	usr, _ := user.Current()
	dir := usr.HomeDir
	return dir + "/.local/bin/chifra/" + part
}

// EstablishCachePaths sets up the cache folders and subfolders. It only returns if it succeeds.
func EstablishCachePaths(cachePath string) {
	cacheFolders := []string{
		"abis", "blocks", "monitors", "names", "objs", "prices",
		"recons", "slurps", "tmp", "traces", "txs",
	}

	_, err := os.Stat(path.Join(cachePath, cacheFolders[len(cacheFolders)-1]))
	if err == nil {
		// If the last path already exists, assume we've been here before
		return
	}

	if err := file.EstablishFolders(cachePath, cacheFolders); err != nil {
		log.Fatal(err)
	}
}

// EstablishIndexPaths sets up the index path and subfolders. It only returns if it succeeds.
func EstablishIndexPaths(indexPath string) {
	indexFolders := []string{
		"blooms", "finalized", "ripe", "staging", "unripe",
	}

	_, err := os.Stat(path.Join(indexPath, indexFolders[len(indexFolders)-1]))
	if err == nil {
		// If the last path already exists, assume we've been here before
		return
	}

	if err := file.EstablishFolders(indexPath, indexFolders); err != nil {
		log.Fatal(err)
	}
}
