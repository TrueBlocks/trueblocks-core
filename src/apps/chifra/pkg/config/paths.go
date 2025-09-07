// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package config

import (
	"os"
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

// MustGetPathToChainConfig returns the chain-specific config folder ignoring errors
func MustGetPathToChainConfig(chain string) string {
	path, _ := PathToChainConfig(chain)
	return path
}

// PathToChainConfig returns the chain-specific config folder
func PathToChainConfig(chain string) (string, error) {
	// We always need a chain
	if len(chain) == 0 {
		chain = GetSettings().DefaultChain
	}
	ret := PathToRootConfig()

	// Our configuration files are always in ./config folder relative to top most folder
	cfgFolder := filepath.Join(ret, "config", chain)
	_, err := os.Stat(cfgFolder)
	return cfgFolder, err
}

// PathToManifestFile returns the path to the manifest database per chain
func PathToManifestFile(chain string) string {
	return filepath.Join(PathToIndex(chain), "manifest.json")
}

// PathToTimestamps returns the path to the timestamps database per chain
func PathToTimestamps(chain string) string {
	return filepath.Join(PathToIndex(chain), "ts.bin")
}

// PathToIndex returns the one and only indexPath
func PathToIndex(chain string) string {
	// We need the index path from either XDG which dominates or the config file
	indexPath, err := pathFromXDG("XDG_CACHE_HOME")
	if err != nil {
		logger.Panic(err)
	} else if len(indexPath) == 0 {
		indexPath = trueBlocksConfig.Settings.IndexPath
	}

	// Probably already true, but can't hurt to be sure
	if filepath.Base(indexPath) != "unchained" {
		indexPath = filepath.Join(indexPath, "unchained")
	}

	// We always have to have a chain...
	if len(chain) == 0 {
		chain = trueBlocksConfig.Settings.DefaultChain
	}

	// We know what we want, create it if it doesn't exist and return it
	newPath := filepath.Join(indexPath, chain)
	EstablishIndexPaths(newPath)
	return newPath
}

// PathToCache returns the one and only cachePath
func PathToCache(chain string) string {
	// We need the index path from either XDG which dominates or the config file
	cachePath, err := pathFromXDG("XDG_CACHE_HOME")
	if err != nil {
		logger.Panic(err)
	} else if len(cachePath) == 0 {
		cachePath = GetSettings().CachePath
	}

	// Probably already true, but can't hurt to be sure
	if filepath.Base(cachePath) != "cache" {
		cachePath = filepath.Join(cachePath, "cache")
	}

	// We always have to have a chain...
	if len(chain) == 0 {
		chain = GetSettings().DefaultChain
	}

	// We know what we want, create it if it doesn't exist and return it
	newPath := filepath.Join(cachePath, chain)
	EstablishCachePaths(newPath)
	return newPath
}

// EstablishCachePaths sets up the cache folders and subfolders. It only returns if it succeeds.
func EstablishCachePaths(cachePath string) {
	folders := []string{
		"abis",
		"monitors",
		filepath.Join("monitors", "staging"),
		"names",
		"tmp",
		"v1",
		filepath.Join("v1", "abis"),
	}

	if err := file.EstablishFolders(cachePath, folders); err != nil {
		logger.Panic(err)
	}
}

// EstablishIndexPaths sets up the index path and subfolders. It only returns if it succeeds.
func EstablishIndexPaths(indexPath string) {
	folders := []string{
		"blooms", "finalized", "maps", "ripe", "staging", "unripe",
	}

	_, err := os.Stat(filepath.Join(indexPath, folders[len(folders)-1]))
	if err == nil {
		// If the last path already exists, assume we've been here before
		return
	}

	if err := file.EstablishFolders(indexPath, folders); err != nil {
		logger.Panic(err)
	}
}
