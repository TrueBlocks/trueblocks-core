// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package cache

import (
	"io/fs"
	"path/filepath"
)

type IndexFileInfo struct {
	Type  CacheType
	Path  string
	Range FileRange
}

func WalkCacheFolder(chain string, cacheType CacheType, filenameChan chan<- IndexFileInfo) {
	defer func() {
		filenameChan <- IndexFileInfo{Type: None}
	}()

	cachePath := NewCachePath(chain, cacheType)
	filepath.Walk(cachePath.GetFullPath(""), func(path string, info fs.FileInfo, err error) error {
		if err != nil {
			// If the scraper is running, this will sometimes send an error for a file, for example, that existed
			// when it was first seen, but the scraper deletes before this call. We ignore any file system errors
			// this routine, but if we experience problems, we can uncomment this line
			// fmt.Printf("prevent panic by handling failure accessing a path %q: %v\n", path, err)
			return err
		}
		if !info.IsDir() {
			fR, _ := RangeFromFilename(path)
			filenameChan <- IndexFileInfo{Type: cacheType, Path: path, Range: fR}
		}
		return nil
	})
}
