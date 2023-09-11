// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package cmd

import (
	"os"
	"os/user"
	"path/filepath"
	"runtime"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

// Initialize makes sure everything is ready to run. These routines don't return if they aren't
func Initialize() bool {
	if os.Getenv("NO_COLOR") == "true" {
		colors.ColorsOff()
	}

	VerifyOs()
	VerifyMigrations()

	return true
}

// VerifyOs will panic if the operating system isn't cooperating
func VerifyOs() {
	userOs := runtime.GOOS
	if userOs == "windows" {
		logger.Fatal("Windows is not supported\n")
	}

	if userOs != "linux" && userOs != "darwin" {
		logger.Fatal("Unsupported operating system: ", userOs, "\n")
	}

	_, err := user.Current()
	if err != nil {
		logger.Fatal("Could not read user home directory\n")
	}
}

const shouldNotExist string = `

	A configuration file or folder ({0}) exists in an old location. Please follow
	all migrations up to {1} to continue.

	See https://github.com/TrueBlocks/trueblocks-core/blob/develop/MIGRATIONS.md

	[{VERSION}]

`

const notExist string = `

	The configuration file or folder ({0})
	does not exist. See https://trueblocks.io/docs/install/install-core/.

	[{VERSION}]

`

const noChains string = `

	The root configuration file ({0})
	does not contain a list of chains. Please follow all migrations up to {1} to continue.

	See https://github.com/TrueBlocks/trueblocks-core/blob/develop/MIGRATIONS.md

	[{VERSION}]

`

const backVersion string = `

	An outdated version of a configration file was found. Please carefully follow 
	migration {0} before proceeding.

	See https://github.com/TrueBlocks/trueblocks-core/blob/develop/MIGRATIONS.md

	[{VERSION}]

`

// VerifyMigrations will panic if the installation is not properly migrated
func VerifyMigrations() {
	// Allow status and config routes to aide user in migrating...
	isStatus := false
	isConfigPaths := false
	for _, arg := range os.Args {
		if arg == "status" {
			isStatus = true
		} else if arg == "config" {
			isConfigPaths = true
		} else if arg == "help" {
			return
		} else if isStatus {
			isStatus = false // only chifra status with no options is okay
		} else if isConfigPaths && arg != "--paths" {
			isConfigPaths = false // only chifra config --paths is okay
		}
	}
	if isStatus || isConfigPaths {
		return
	}

	user, _ := user.Current()

	// The old $HOME/.quickBlocks folder should not exist...
	if _, err := os.Stat(filepath.Join(user.HomeDir, ".quickBlocks")); err == nil {
		msg := strings.Replace(shouldNotExist, "{0}", "{~/.quickBlocks}", -1)
		msg = strings.Replace(msg, "[{VERSION}]", versionText, -1)
		msg = strings.Replace(msg, "{1}", "{v0.09.0}", -1)
		msg = strings.Replace(msg, "{", colors.Green, -1)
		msg = strings.Replace(msg, "}", colors.Off, -1)
		logger.Fatal(msg)
	}

	// Both the config folder...
	configFolder := config.GetPathToRootConfig()
	if _, err := os.Stat(configFolder); err != nil {
		msg := strings.Replace(notExist, "{0}", "{"+configFolder+"}", -1)
		msg = strings.Replace(msg, "[{VERSION}]", versionText, -1)
		msg = strings.Replace(msg, "{", colors.Green, -1)
		msg = strings.Replace(msg, "}", colors.Off, -1)
		logger.Fatal(msg)
	}

	// ...and the config file better exist.
	configFile := filepath.Join(configFolder + "trueBlocks.toml")
	if _, err := os.Stat(configFile); err != nil {
		msg := strings.Replace(notExist, "{0}", "{"+configFile+"}", -1)
		msg = strings.Replace(msg, "[{VERSION}]", versionText, -1)
		msg = strings.Replace(msg, "{", colors.Green, -1)
		msg = strings.Replace(msg, "}", colors.Off, -1)
		logger.Fatal(msg)
	}

	// ...and some chains...
	if !config.HasChains() {
		msg := strings.Replace(noChains, "{0}", "{"+configFile+"}", -1)
		msg = strings.Replace(msg, "{1}", "{v0.25.0}", -1)
		msg = strings.Replace(msg, "[{VERSION}]", versionText, -1)
		msg = strings.Replace(msg, "{", colors.Green, -1)
		msg = strings.Replace(msg, "}", colors.Off, -1)
		logger.Fatal(msg)
	}

	// We need to find the chain configuration path
	chainConfigPath := config.GetPathToChainConfig("")
	if _, err := os.Stat(chainConfigPath); err != nil {
		msg := strings.Replace(notExist, "{0}", "{"+chainConfigPath+"}", -1)
		msg = strings.Replace(msg, "[{VERSION}]", versionText, -1)
		msg = strings.Replace(msg, "{", colors.Green, -1)
		msg = strings.Replace(msg, "}", colors.Off, -1)
		logger.Fatal(msg)
	}

	// Make sure they've completed migrations prior to v1.0.0
	items := []string{
		"blocks",
		"objs",
		"recons",
		"slurps",
		"traces",
		"txs",
	}
	for _, item := range items {
		itemPath := filepath.Join(config.GetPathToCache(""), item)
		if _, err := os.Stat(itemPath); err == nil {
			msg := strings.Replace(shouldNotExist, "{0}", "{"+itemPath+"}", -1)
			msg = strings.Replace(msg, "[{VERSION}]", versionText, -1)
			msg = strings.Replace(msg, "{1}", "{v1.0.0}", -1)
			msg = strings.Replace(msg, "{", colors.Green, -1)
			msg = strings.Replace(msg, "}", colors.Off, -1)
			logger.Fatal(msg)
		}
	}

	// We need at least this version...
	requiredVersion := "v1.0.0-release"
	if !config.IsAtLeastVersion(requiredVersion) {
		msg := strings.Replace(backVersion, "{0}", "{"+requiredVersion+"}", -1)
		msg = strings.Replace(msg, "[{VERSION}]", versionText, -1)
		msg = strings.Replace(msg, "{", colors.Green, -1)
		msg = strings.Replace(msg, "}", colors.Off, -1)
		logger.Fatal(msg)
	}
}
