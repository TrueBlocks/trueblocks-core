// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package file

import (
	"os"
	"path"
)

// establishFolders creates the rootPath and any subfolders
func EstablishFolders(rootPath string, folders []string) error {
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

	for _, folder := range folders {
		err := establishFolder(path.Join(rootPath, folder))
		if err != nil {
			return err
		}
	}

	return nil
}

// establishFolder creates folders given a list of folders
func establishFolder(subdirPath string) error {
	_, err := os.Stat(subdirPath)
	if err != nil {
		if os.IsNotExist(err) {
			err = os.Mkdir(subdirPath, 0755)
			if err != nil {
				return err
			}
		} else {
			// If there's an error other than not exist...we fail
			return err
		}
	}
	return nil
}
