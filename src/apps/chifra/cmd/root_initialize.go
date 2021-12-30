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
	EstablishCaches()

	return true
}

var errCustomFolderMissing = `Attempt to create customized indexPath (%s) failed.
Please create the folder or adjust the setting by editing $CONFIG/trueBlocks.toml.
`

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

	// configPath := config.GetConfigPath("chains")
	// if _, err := os.Stat(configPath); err != nil {
	// 	msg := "\n\n"
	//     msg += "\tThe multi-chain folder ($CONFIG/chains) was not found. Please complete migration v0.23.0.\n"
	// 	msg += "\tSee https://github.com/TrueBlocks/trueblocks-core/blob/develop/MIGRATIONS.md\n"
	//     msg += "\n";
	// 	log.Fatalf(msg)
	//}
}

// EstablishCaches sets up the cache folders and subfolders
func EstablishCaches() {
	cachePath := config.ReadTrueBlocks().Settings.CachePath
	cacheFolders := []string{
		"abis", "blocks", "monitors", "names", "objs", "prices",
		"recons", "slurps", "tmp", "traces", "txs",
	}
	if err := establishFolders(cachePath, cacheFolders); err != nil {
		log.Fatal(err)
	}

	indexPath := config.ReadTrueBlocks().Settings.IndexPath
	indexFolders := []string{
		"blooms", "finalized", "ripe", "staging", "unripe",
	}
	if err := establishFolders(indexPath, indexFolders); err != nil {
		log.Fatal(err)
	}
}

// establishFolders creates the rootPath and any subfolders
func establishFolders(rootPath string, folders []string) error {
	_, err := os.Stat(rootPath)
	if err != nil {
		if os.IsNotExist(err) {
			// Create the rootPath if it doesn't exist
			err = os.Mkdir(rootPath, 0755)
			if err != nil {
				return err
			}

			createSubfolders(rootPath, folders)

		} else {
			// If there's an error other than not exist...we fail
			return err
		}
	}
	return nil
}

// createSubfolders creates folders given a list of folders
func createSubfolders(rootPath string, folders []string) error {
	for _, folder := range folders {
		subdirPath := path.Join(rootPath, folder)
		_, err := os.Stat(subdirPath)
		if err != nil {
			if os.IsNotExist(err) {
				err = os.Mkdir(subdirPath, 0755)
				if err != nil {
					return err
				}
			} else {
				return err
			}
		}
	}
	return nil
}
