// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package cache

import (
	"io/fs"
	"path/filepath"
	"regexp"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

func WalkCacheFolder(chain string, cacheType CacheType, regExp *regexp.Regexp, filenameChan chan<- CacheFileInfo) {
	path := GetRootPathFromCacheType(chain, cacheType)
	walkFolder(path, cacheType, regExp, filenameChan)
}

func walkFolder(path string, cacheType CacheType, regExp *regexp.Regexp, filenameChan chan<- CacheFileInfo) {
	defer func() {
		filenameChan <- CacheFileInfo{Type: Cache_NotACache}
	}()

	filepath.Walk(path, func(path string, info fs.FileInfo, err error) error {
		if err != nil {
			// If the scraper is running, this will sometimes send an error for a file, for example, that existed
			// when it was first seen, but the scraper deletes before this call. We ignore any file system errors
			// this routine, but if we experience problems, we can uncomment this line
			// fmt.Printf("prevent panic by handling failure accessing a path %q: %v\n", path, err)
			return err
		}

		if info.IsDir() {
			filenameChan <- CacheFileInfo{Type: cacheType, Path: path, IsDir: true}

		} else if regExp == nil || regExp.MatchString(info.Name()) {
			rng := base.RangeFromFilename(path)
			filenameChan <- CacheFileInfo{Type: cacheType, Path: path, Range: rng}
		}

		return nil
	})
}

type CacheFileInfo struct {
	Type  CacheType
	Path  string
	Range base.FileRange
	IsDir bool
}
