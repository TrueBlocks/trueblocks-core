// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package file

import (
	"io/fs"
	"os"
	"path/filepath"
)

func GetNewestInDirectory(directory string) (fileInfo os.FileInfo, err error) {
	err = filepath.WalkDir(directory, func(path string, entry fs.DirEntry, err error) error {
		if err != nil {
			return err
		}
		if entry.IsDir() {
			return nil
		}
		currentFileInfo, err := entry.Info()
		if err != nil {
			return err
		}
		if fileInfo == nil || currentFileInfo.ModTime().After(fileInfo.ModTime()) {
			fileInfo = currentFileInfo
		}
		return nil
	})
	return
}

// func getLastModTs(fileName string) (int64, error) {
// 	info, err := os.Stat(fileName)
// 	if err != nil {
// 		return 0, err
// 	}
// 	modTime := info.ModTime()
// 	return modTime.Unix(), nil
// }
