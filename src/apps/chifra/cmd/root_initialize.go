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

	if len(config.OsToPath[userOs]) == 0 {
		log.Fatalln("Unsupported operating system: ", userOs)
	}

	_, err := user.Current()
	if err != nil {
		log.Fatalln("Could not read user home directory")
	}
}

// VerifyMigrations will panic if the installation is not properly migrated
func VerifyMigrations() {
	user, _ := user.Current()

	if _, err := os.Stat(path.Join(user.HomeDir, ".quickBlocks")); err == nil {
		msg := "\n\n"
		msg += "\tAn old configuration folder (~/.quickBlocks) exists. Please complete migration v0.09.0.\n"
		msg += "\tSee https://github.com/TrueBlocks/trueblocks-core/blob/develop/MIGRATIONS.md\n"
		msg += "\n"
		log.Fatalf(msg)
	}

	// Search for MultiChain
	// configPath := config.GetPath ToConfig("chains")
	// if _, err := os.Stat(configPath); err != nil {
	// 	msg := "\n\n"
	//     msg += "\tThe multi-chain folder ($CONFIG/chains) was not found. Please complete migration v0.23.0.\n"
	// 	msg += "\tSee https://github.com/TrueBlocks/trueblocks-core/blob/develop/MIGRATIONS.md\n"
	//     msg += "\n";
	// 	log.Fatalf(msg)
	//}
}
