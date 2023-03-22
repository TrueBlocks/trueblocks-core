// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package cache

import (
	"io/fs"
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/paths"
)

type IndexFileInfo struct {
	Type  paths.CacheType
	Path  string
	Range base.FileRange
}

func WalkIndexFolder(chain string, cacheType paths.CacheType, filenameChan chan<- IndexFileInfo) {
	defer func() {
		filenameChan <- IndexFileInfo{Type: paths.None}
	}()

	path := filepath.Join(config.GetPathToIndex(chain), tailFolder(chain, cacheType))
	if cacheType == paths.Index_Bloom {
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

func tailFolder(chain string, ct paths.CacheType) string {
	descrs := map[paths.CacheType]string{
		paths.None:          "unknown",
		paths.Index_Bloom:   "blooms",
		paths.Index_Final:   "finalized",
		paths.Index_Staging: "staging",
		paths.Index_Ripe:    "ripe",
		paths.Index_Unripe:  "unripe",
		paths.Cache_Abis:    "abis",
	}
	return descrs[ct]
}

type CacheFileInfo struct {
	Type paths.CacheType
	Path string
}

func WalkCacheFolder(chain string, cacheType paths.CacheType, filenameChan chan<- CacheFileInfo) {
	defer func() {
		filenameChan <- CacheFileInfo{Type: paths.None}
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
