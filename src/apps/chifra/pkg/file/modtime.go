// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package file

import (
	"io/fs"
	"os"
	"path/filepath"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/logger"
)

func MustGetLatestFileTime(folders ...string) time.Time {
	if len(folders) == 0 {
		return time.Now()
	}

	getTimeOfNewestFile := func(folder string) time.Time {
		if info, err := GetNewestInDirectory(folder); err != nil || info == nil {
			if info == nil {
				logger.Warn("latest file time skipped", "error", err)
			} else {
				logger.Error("error getting latest file time:", err)
			}
			return time.Now()
		} else {
			return info.ModTime()
		}
	}

	var newestTime time.Time
	for _, folder := range folders {
		fileTime := getTimeOfNewestFile(folder)
		if fileTime.After(newestTime) {
			newestTime = fileTime
		}
	}

	return newestTime
}

func GetNewestInDirectory(directory string) (fileInfo os.FileInfo, err error) {
	err = filepath.WalkDir(directory, func(path string, entry fs.DirEntry, err error) error {
		_ = path
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

func IsLaterThan(fn1, fn2 string) (bool, error) {
	info1, err := os.Stat(fn1)
	if err != nil {
		return false, err
	}
	info2, err := os.Stat(fn2)
	if err != nil {
		return false, err
	}
	return info1.ModTime().After(info2.ModTime()), nil
}

func GetModTime(fn string) (time.Time, error) {
	if info, err := os.Stat(fn); err != nil {
		return time.Time{}, err
	} else {
		return info.ModTime(), nil
	}
}
