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

// VerifyMigrations will panic if the installation is not properly migrated
func VerifyMigrations() {
	if isPermitted() {
		// Allow certain status and config routes to pass so as to aide user in migrating...
		return
	}

	user, _ := user.Current()

	const doesNotExist string = `0002. A config item ({0}) is missing. See {https://trueblocks.io/docs/install/install-core/}.`
	const shouldNotExist string = `0003. A config item ({0}) exists but should not. See {https://trueblocks.io/docs/install/install-core/}.`
	const noChains string = `0004. The configuration file ({0}) contains no chain specifications. See {https://trueblocks.io/docs/install/install-core/}.`

	// The old $HOME/.quickBlocks folder should not exist...
	if _, err := os.Stat(filepath.Join(user.HomeDir, ".quickBlocks")); err == nil {
		msg := strings.Replace(shouldNotExist, "{0}", "{~/.quickBlocks}", -1)
		msg = colors.ColoredWith(msg, colors.Yellow)
		logger.Fatal(msg)
	}

	// Both the config folder...
	configFolder := config.PathToRootConfig()
	if _, err := os.Stat(configFolder); err != nil {
		msg := strings.Replace(doesNotExist, "{0}", "{"+configFolder+"}", -1)
		msg = colors.ColoredWith(msg, colors.Yellow)
		logger.Fatal(msg)
	}

	// ...and the config file better exist.
	configFile := filepath.Join(configFolder + "trueBlocks.toml")
	if _, err := os.Stat(configFile); err != nil {
		msg := strings.Replace(doesNotExist, "{0}", "{"+configFile+"}", -1)
		msg = colors.ColoredWith(msg, colors.Yellow)
		logger.Fatal(msg)
	}

	// ...and some chains...
	chainArray := config.GetChains()
	if len(chainArray) == 0 {
		msg := strings.Replace(noChains, "{0}", "{"+configFile+"}", -1)
		msg = colors.ColoredWith(msg, colors.Yellow)
		logger.Fatal(msg)
	}

	// We need to find the chain configuration path
	chainConfigPath := config.MustGetPathToChainConfig("")
	if _, err := os.Stat(chainConfigPath); err != nil {
		msg := strings.Replace(doesNotExist, "{0}", "{"+chainConfigPath+"}", -1)
		msg = colors.ColoredWith(msg, colors.Yellow)
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
		itemPath := filepath.Join(config.PathToCache(""), item)
		if _, err := os.Stat(itemPath); err == nil {
			msg := strings.Replace(shouldNotExist, "{0}", "{"+itemPath+"}", -1)
			msg = colors.ColoredWith(msg, colors.Yellow)
			logger.Fatal(msg)
		}
	}
}

func isPermitted() bool {
	isStatus := false
	isConfig := false
	hasPaths := false
	hasEdit := false
	cnt := len(os.Args)
	for _, arg := range os.Args {
		if arg == "help" {
			return false
		} else if arg == "status" {
			isStatus = true
		} else if arg == "config" {
			isConfig = true
		} else if arg == "--paths" {
			hasPaths = true
		} else if arg == "edit" {
			hasEdit = true
		} else if arg != "--verbose" {
			isStatus = false
			isConfig = false
		} else {
			cnt-- // allow --verbose
		}
	}

	if isStatus && cnt == 2 {
		return true
	}

	return isConfig && (hasPaths || hasEdit) && cnt < 4
}
