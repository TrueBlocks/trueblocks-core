// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package cmd

import (
	"log"
	"os"
	"os/user"
	"path"
	"runtime"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
)

// Initalize makes sure everything is ready to run. These routines don't return if they aren't
func Initialize() bool {
	VerifyOs()
	VerifyMigrations()

	return true
}

// VerifyOs will panic if the operating system isn't cooperating
func VerifyOs() {
	userOs := runtime.GOOS
	if userOs == "windows" {
		log.Fatalln("Windows is not supported")
	}

	if userOs != "linux" && userOs != "darwin" {
		log.Fatalln("Unsupported operating system: ", userOs)
	}

	_, err := user.Current()
	if err != nil {
		log.Fatalln("Could not read user home directory")
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
	does not exist. See https://trueblocks.io/docs/install/install-trueblocks/.

	[{VERSION}]

`

const noChains string = `

	The root configuration file ({0})
	does not contain a list of chains. Please follow all migrations up to {1} to continue.

	See https://github.com/TrueBlocks/trueblocks-core/blob/develop/MIGRATIONS.md

	[{VERSION}]

`

// VerifyMigrations will panic if the installation is not properly migrated
func VerifyMigrations() {
	user, _ := user.Current()

	// The old $HOME/.quickBlocks folder should not exist...
	if _, err := os.Stat(path.Join(user.HomeDir, ".quickBlocks")); err == nil {
		msg := strings.Replace(shouldNotExist, "{0}", "{~/.quickBlocks}", -1)
		msg = strings.Replace(msg, "[{VERSION}]", versionText, -1)
		msg = strings.Replace(msg, "{1}", "{v0.09.0}", -1)
		msg = strings.Replace(msg, "{", colors.Green, -1)
		msg = strings.Replace(msg, "}", colors.Off, -1)
		log.Fatalf(msg)
	}

	// Both the config folder...
	configFolder := config.GetPathToRootConfig()
	if _, err := os.Stat(configFolder); err != nil {
		msg := strings.Replace(notExist, "{0}", "{"+configFolder+"}", -1)
		msg = strings.Replace(msg, "[{VERSION}]", versionText, -1)
		msg = strings.Replace(msg, "{", colors.Green, -1)
		msg = strings.Replace(msg, "}", colors.Off, -1)
		log.Fatalf(msg)
	}

	// ...and the config file better exist.
	configFile := path.Join(configFolder + "trueBlocks.toml")
	if _, err := os.Stat(configFile); err != nil {
		msg := strings.Replace(notExist, "{0}", "{"+configFile+"}", -1)
		msg = strings.Replace(msg, "[{VERSION}]", versionText, -1)
		msg = strings.Replace(msg, "{", colors.Green, -1)
		msg = strings.Replace(msg, "}", colors.Off, -1)
		log.Fatalf(msg)
	}

	if !config.HasChains() {
		msg := strings.Replace(noChains, "{0}", "{"+configFile+"}", -1)
		msg = strings.Replace(msg, "{1}", "{v0.25.0}", -1)
		msg = strings.Replace(msg, "[{VERSION}]", versionText, -1)
		msg = strings.Replace(msg, "{", colors.Green, -1)
		msg = strings.Replace(msg, "}", colors.Off, -1)
		log.Fatalf(msg)
	}

	// If any of the following folders or files exist, the user has not
	// completed migration 0.25.0. Tell them to do so and quit.
	// TODO(tjayrush): This can be removed at some point
	items := []string{
		"manifest",
		"mocked",
		"names",
		"blockScrape.toml",
		"ethNames.toml",
		"ethslurp.toml",
		"fireStorm.toml",
		"whenBlock.toml",
		"poloniex_USDT_ETH.bin.gz",
		"ts.bin.gz",
		"cache/abis/",
		"cache/blocks/",
		"cache/monitors/",
		"cache/names/",
		"cache/objs/",
		"cache/prices/",
		"cache/recons/",
		"cache/slurps/",
		"cache/tmp/",
		"cache/traces/",
		"cache/txs/",
		"unchained/staging/",
		"unchained/unripe/",
		"unchained/ripe/",
		"unchained/finalized/",
		"unchained/blooms/",
		"unchained/maps/",
		"unchained/ts.bin",
	}
	for _, item := range items {
		itemPath := path.Join(configFolder + item)
		if _, err := os.Stat(itemPath); err == nil {
			msg := strings.Replace(shouldNotExist, "{0}", "{./"+item+"}", -1)
			msg = strings.Replace(msg, "[{VERSION}]", versionText, -1)
			msg = strings.Replace(msg, "{1}", "{v0.25.0}", -1)
			msg = strings.Replace(msg, "{", colors.Green, -1)
			msg = strings.Replace(msg, "}", colors.Off, -1)
			log.Fatalf(msg)
		}
	}

	chainConfigPath := config.GetPathToChainConfig("")
	if _, err := os.Stat(chainConfigPath); err != nil {
		msg := strings.Replace(notExist, "{0}", "{"+chainConfigPath+"}", -1)
		msg = strings.Replace(msg, "[{VERSION}]", versionText, -1)
		msg = strings.Replace(msg, "{", colors.Green, -1)
		msg = strings.Replace(msg, "}", colors.Off, -1)
		log.Fatalf(msg)
	}
}
