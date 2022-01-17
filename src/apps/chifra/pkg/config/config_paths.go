// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package config

import (
	"log"
	"os"
	"os/user"
	"path"
	"runtime"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

var OsToPath = map[string]string{
	"linux":  ".local/share/trueblocks",
	"darwin": "Library/Application Support/TrueBlocks",
}

// GetPathToChainConfig_newOff returns the chain-specific config folder
func GetPathToChainConfig_newOff() string {
	ret := GetPathToRootConfig()
	return ret
}

// GetPathToChainConfig1 returns the chain-specific config folder
func GetPathToChainConfig1(chain string) string {
	ret := GetPathToRootConfig()
	return path.Join(ret, "config/", chain) + "/"
}

// TODO: Search for PathAccessor
// GetPathToRootConfig returns the path where to find configuration files
func GetPathToRootConfig() string {
	xdg := os.Getenv("XDG_CONFIG_HOME")
	if len(xdg) > 0 && xdg[0] == '/' {
		return path.Join(xdg, "") + "/"
	}

	// The migration code will have already checked for invalid operating systems (i.e. Windows)
	userOs := runtime.GOOS
	if len(os.Getenv("TEST_OS")) > 0 {
		userOs = os.Getenv("TEST_OS")
	}

	user, _ := user.Current()
	return path.Join(user.HomeDir, OsToPath[userOs]) + "/"
}

// GetPathToIndex returns the one and only cachePath
func GetPathToIndex(chain string) string {
	if len(chain) == 0 {
		chain = "mainnet"
	}
	newPath := ""
	xdg := os.Getenv("XDG_CACHE_HOME")
	if len(xdg) > 0 && xdg[0] == '/' {
		if !strings.Contains(xdg, "/unchained") {
			xdg = path.Join(xdg, "unchained")
		}
		newPath = path.Join(xdg, chain) + "/"
	} else {
		indexPath := readTrueBlocks().Settings.IndexPath
		newPath = path.Join(indexPath) + "/"
		// newPath = path.Join(indexPath, chain) + "/"
	}
	EstablishCachePaths(newPath)
	return newPath
}

// GetPathToCache returns the one and only cachePath
func GetPathToCache(chain string) string {
	if len(chain) == 0 {
		chain = "mainnet"
	}
	newPath := ""
	xdg := os.Getenv("XDG_CACHE_HOME")
	if len(xdg) > 0 && xdg[0] == '/' {
		if !strings.Contains(xdg, "/cache") {
			xdg = path.Join(xdg, "cache")
		}
		newPath = path.Join(xdg, chain) + "/"
	} else {
		cachePath := readTrueBlocks().Settings.CachePath
		newPath = path.Join(cachePath, chain) + "/"
	}
	EstablishCachePaths(newPath)
	return newPath
}

// TODO: BOGUS
// // GetPathToIndex1 returns the one and only cachePath
// func GetPathToIndex1(chain string) string {
// 	if len(chain) == 0 {
// 		chain = "mainnet"
// 	}
// 	xdg := os.Getenv("XDG_CACHE_HOME")
// 	if len(xdg) > 0 && xdg[0] == '/' {
// 		if !strings.Contains(xdg, "/unchained") {
// 			xdg = path.Join(xdg, "unchained")
// 		}
// 		return path.Join(xdg, chain) + "/"
// 	}
// 	return path.Join(readTrueBlocks().Settings.IndexPath, chain) + "/"
// }

// GetRpcProvider returns the RPC provider for a chain
func GetRpcProvider() string {
	return readTrueBlocks().Settings.RpcProvider
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
