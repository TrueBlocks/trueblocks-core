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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

var OsToPath = map[string]string{
	"linux":  ".local/share/trueblocks",
	"darwin": "Library/Application Support/TrueBlocks",
}

// TODO: Search for PathAccessor
// GetPathToConfig returns the path where to find configuration files (appends chain if told to do so)
func GetPathToConfig(withChain bool) string {
	chain := ""
	if withChain {
		// fmt.Println(os.Getenv("TEST_CHAIN"))
		chain = GetChain("mainnet")
		if len(os.Getenv("TEST_CHAIN")) > 0 {
			chain = os.Getenv("TEST_CHAIN") + "/"
		}
		chain = "config/" + chain
	}

	xdg := os.Getenv("XDG_CONFIG_HOME")
	if len(xdg) > 0 && xdg[0] == '/' {
		return path.Join(xdg, chain) + "/"
	}

	// The migration code will have already checked for invalid operating systems (i.e. Windows)
	userOs := runtime.GOOS
	if len(os.Getenv("TEST_OS")) > 0 {
		userOs = os.Getenv("TEST_OS")
	}

	user, _ := user.Current()
	return path.Join(user.HomeDir, OsToPath[userOs], chain) + "/"
}

// GetPathToCache returns the one and only cachePath
func GetPathToCache() string {
	return path.Join(ReadTrueBlocks().Settings.CachePath) + "/"
}

// GetPathToIndex returns the one and only cachePath
func GetPathToIndex() string {
	return path.Join(ReadTrueBlocks().Settings.IndexPath) + "/"
}

// GetRpcProvider returns the RPC provider for a chain
func GetRpcProvider() string {
	return ReadTrueBlocks().Settings.RpcProvider
}

// GetChain returns the value of the chain parameter or the default
func GetChain(def string) string {
	if len(def) == 0 {
		def = "mainnet"
	}
	return def + "/" // path.Join(ReadTrueBlocks().Settings.Chain) + "/"
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

// EstablishCachePaths sets up the cache index and subfolders. It only returns if it succeeds.
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
