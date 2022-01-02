// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package cache

import (
	"log"
	"os"
	"path"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
)

// EstablishCaches sets up the cache folders and subfolders
func EstablishCaches() {
	cachePath := config.GetCachePath()
	cacheFolders := []string{
		"abis", "blocks", "monitors", "names", "objs", "prices",
		"recons", "slurps", "tmp", "traces", "txs",
	}
	if err := establishFolders(cachePath, cacheFolders); err != nil {
		log.Fatal(err)
	}

	indexPath := config.GetIndexPath()
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

		} else {
			// If there's an error other than not exist...we fail
			return err
		}
	}

	return createSubfolders(rootPath, folders)
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
