// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package file

import (
	"os"
	"path"
)

// EstablishFolders creates the rootPath and any subfolders
func EstablishFolders(rootPath string, folders []string) error {
	err := EstablishFolder(rootPath)
	if err != nil {
		return err
	}

	if folders != nil {
		for _, folder := range folders {
			err := EstablishFolder(path.Join(rootPath, folder))
			if err != nil {
				return err
			}
		}
	}

	return nil
}

// EstablishFolder creates folders given a list of folders
func EstablishFolder(rootPath string) error {
	_, err := os.Stat(rootPath)
	if err != nil {
		if os.IsNotExist(err) {
			err = os.Mkdir(rootPath, 0755)
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
