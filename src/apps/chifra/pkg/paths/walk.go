// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package paths

import (
	"io/fs"
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
)

type IndexFileInfo struct {
	Type  CacheType
	Path  string
	Range base.FileRange
}

func WalkIndexFolder(chain string, cacheType CacheType, filenameChan chan<- IndexFileInfo) {
	defer func() {
		filenameChan <- IndexFileInfo{Type: None}
	}()

	path := filepath.Join(config.GetPathToIndex(chain), tailFolder(chain, cacheType))
	if cacheType == Index_Bloom {
		path = ToBloomPath(path)
	}

	filepath.Walk(path, func(path string, info fs.FileInfo, err error) error {
		if err != nil {
			// If the scraper is running, this will sometimes send an error for a file, for example, that existed
			// when it was first seen, but the scraper deletes before this call. We ignore any file system errors
			// this routine, but if we experience problems, we can uncomment this line
			// fmt.Printf("prevent panic by handling failure accessing a path %q: %v\n", path, err)
			return err
		}
		if !info.IsDir() {
			rng := base.RangeFromFilename(path)
			filenameChan <- IndexFileInfo{Type: cacheType, Path: path, Range: rng}
		}
		return nil
	})
}

func tailFolder(chain string, ct CacheType) string {
	descrs := map[CacheType]string{
		None:          "unknown",
		Index_Bloom:   "blooms",
		Index_Final:   "finalized",
		Index_Staging: "staging",
		Index_Ripe:    "ripe",
		Index_Unripe:  "unripe",
		Cache_Abis:    "abis",
	}
	return descrs[ct]
}

type CacheFileInfo struct {
	Type CacheType
	Path string
}

func WalkCacheFolder(chain string, cacheType CacheType, filenameChan chan<- CacheFileInfo) {
	defer func() {
		filenameChan <- CacheFileInfo{Type: None}
	}()

	path := filepath.Join(config.GetPathToCache(chain), tailFolder(chain, cacheType))
	filepath.Walk(path, func(path string, info fs.FileInfo, err error) error {
		if err != nil {
			// If the scraper is running, this will sometimes send an error for a file, for example, that existed
			// when it was first seen, but the scraper deletes before this call. We ignore any file system errors
			// this routine, but if we experience problems, we can uncomment this line
			// fmt.Printf("prevent panic by handling failure accessing a path %q: %v\n", path, err)
			return err
		}

		if !info.IsDir() {
			filenameChan <- CacheFileInfo{Type: cacheType, Path: path}
		}

		return nil
	})
}
