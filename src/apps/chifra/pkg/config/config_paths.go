// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package config

import (
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
func GetPathToConfig(fileName string) string {
	// These values are checked in CheckMigrations and will not proceed if not valid
	userOs := runtime.GOOS
	user, _ := user.Current()
	return path.Join(user.HomeDir, OsToPath[userOs], fileName)
}

// TODO: Search for PathAccessor
// GetPathToCache returns the one and only cachePath
func GetPathToCache() string {
	return ReadTrueBlocks().Settings.CachePath
}

// TODO: Search for PathAccessor
// GetPathToIndex returns the one and only cachePath
func GetPathToIndex() string {
	return ReadTrueBlocks().Settings.IndexPath
}

// TODO: Search for PathAccessor
// GetPathToCommands returns full path the the given tool
func GetPathToCommands(part string) string {
	usr, _ := user.Current()
	dir := usr.HomeDir
	return dir + "/.local/bin/chifra/" + part
}
