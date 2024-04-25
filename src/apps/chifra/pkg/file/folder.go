// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package file

import (
	"fmt"
	"io"
	"os"
	"path/filepath"
	"strings"
)

func IsFolderEmpty(folder string) (bool, error) {
	f, err := os.OpenFile(folder, os.O_RDONLY, 0)
	if err != nil {
		return false, err
	}
	defer f.Close()
	_, err = f.Readdirnames(1)
	if err == io.EOF {
		return true, nil
	}
	return false, err // Either not empty or error, suits both cases
}

// CleanFolder removes any files that may be partial or incomplete
func CleanFolder(chain, rootFolder string, subFolders []string) error {
	for _, f := range subFolders {
		folder := filepath.Join(rootFolder, f)
		// We want to remove whatever is there...
		err := os.RemoveAll(folder)
		if err != nil {
			return err
		}
		// ...but put it back
		_ = EstablishFolder(folder)
	}

	return nil
}

// IsValidFolderName checks if the folder name is valid and does not exist in the current directory.
func IsValidFolderName(folderName string) (bool, error) {
	// Check for invalid characters (example for a simple Unix-like rule)
	// You might need a more specific check depending on your OS requirements.
	if strings.ContainsAny(folderName, "/<>:\"\\|?*") {
		return false, fmt.Errorf("folder name contains invalid characters")
	}

	// Check if folder exists
	if _, err := os.Stat(folderName); err == nil {
		return false, fmt.Errorf("folder already exists")
	} else if !os.IsNotExist(err) {
		return false, err // some other error occurred when checking the folder
	}

	return true, nil
}
