// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package config

import (
	"os"
	"os/user"
	"path"
	"runtime"
)

// TODO: This should not be hard coded
var OsToPath = map[string]string{
	"linux":  ".local/share/trueblocks",
	"darwin": "Library/Application Support/TrueBlocks",
}

// TODO: Search for PathAccessor
// GetPathToConfig returns the path to the directory where the configuration files are
func GetPathToConfig(part string) string {
	chain := ""
	// order matters
	if len(part) == 0 || part[0] != '/' {
		part = "/" + part
	} else {
		chain := GetChain()
		if chain == "/" {
			chain = "mainnet/"
		}
		// fmt.Println("Chain: ", chain)
	}

	xdg := os.Getenv("XDG_DATA_HOME")
	if len(xdg) > 0 && xdg[0] == '/' {
		return path.Join(xdg, chain) + part
	}

	// The migration code will have already checked for invalid operating systems (i.e. Windows)
	userOs := runtime.GOOS
	if len(os.Getenv("TEST_OS")) > 0 {
		userOs = os.Getenv("TEST_OS")
	}

	user, _ := user.Current()
	return path.Join(user.HomeDir, OsToPath[userOs], chain) + part
}

// GetPathToCache returns the one and only cachePath
func GetPathToCache() string {
	return path.Join(ReadTrueBlocks().Settings.CachePath) + "/"
}

// GetPathToIndex returns the one and only cachePath
func GetPathToIndex() string {
	return path.Join(ReadTrueBlocks().Settings.IndexPath) + "/"
}

// GetChain returns the value of the chain parameter
func GetChain() string {
	// if len(os.Getenv("TEST_CHAIN")) > 0 {
	// 	return os.Getenv("TEST_CHAIN")
	// }
	return "/" // path.Join(ReadTrueBlocks().Settings.Chain) + "/"
}

// GetPathToChainConfig returns the path to the chain-specific configuration file
func GetPathToChainConfig(part string) string {
	ret := GetPathToConfig("")
	// fmt.Println("Chain config would be: ", path.Join(ret, GetChain())+"/")
	return ret + part
}

// GetPathToCommands returns full path the the given tool
func GetPathToCommands(part string) string {
	usr, _ := user.Current()
	dir := usr.HomeDir
	return dir + "/.local/bin/chifra/" + part
}
