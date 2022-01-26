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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config/rootConfig"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

// GetPathToChainConfig returns the chain-specific config folder
func GetPathToChainConfig(chain string) string {
	if len(chain) == 0 {
		chain = rootConfig.GetDefaultChain()
	}
	ret := rootConfig.GetPathToRootConfig()
	return path.Join(ret, "config/", chain) + "/"
}

// GetPathToIndex returns the one and only cachePath
func GetPathToIndex(chain string) string {
	newPath := ""

	if len(chain) == 0 {
		chain = rootConfig.GetDefaultChain()
	}

	xdg := os.Getenv("XDG_CACHE_HOME")
	if len(xdg) > 0 && xdg[0] == '/' {
		if !strings.Contains(xdg, "/unchained") {
			xdg = path.Join(xdg, "unchained")
		}
		newPath = path.Join(xdg, chain) + "/"
	} else {
		indexPath := rootConfig.GetIndexPath()
		newPath = path.Join(indexPath, chain) + "/"
	}

	EstablishIndexPaths(newPath)
	return newPath
}

// GetPathToCache returns the one and only cachePath
func GetPathToCache(chain string) string {
	newPath := ""

	if len(chain) == 0 {
		chain = rootConfig.GetDefaultChain()
	}

	xdg := os.Getenv("XDG_CACHE_HOME")
	if len(xdg) > 0 && xdg[0] == '/' {
		if !strings.Contains(xdg, "/cache") {
			xdg = path.Join(xdg, "cache")
		}
		newPath = path.Join(xdg, chain) + "/"
	} else {
		cachePath := rootConfig.GetCachePath()
		newPath = path.Join(cachePath, chain) + "/"
	}

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
