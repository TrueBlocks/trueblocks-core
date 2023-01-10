// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package file

import "os"

func GetNewstInDirectory(directory string) (fileInfo os.FileInfo, err error) {
	files, err := os.ReadDir(directory)
	if err != nil {
		return
	}

	for _, file := range files {
		var currentFileInfo os.FileInfo
		currentFileInfo, err = file.Info()
		if err != nil {
			return
		}

		if fileInfo == nil || currentFileInfo.ModTime().After(fileInfo.ModTime()) {
			fileInfo = currentFileInfo
		}
	}
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
