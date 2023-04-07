// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package cache

import (
	"context"
	"io/fs"
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

func WalkCacheFolder(ctx context.Context, chain string, cacheType CacheType, data interface{}, filenameChan chan<- CacheFileInfo) {
	path := GetRootPathFromCacheType(chain, cacheType)
	walkFolder(ctx, path, cacheType, data, filenameChan)
}

func walkFolder(ctx context.Context, path string, cacheType CacheType, data interface{}, filenameChan chan<- CacheFileInfo) {
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
			filenameChan <- CacheFileInfo{Type: cacheType, Path: path, IsDir: true, Data: data}

		} else {
			// TODO: This does not need to be part of walker. It could be in the caller and sent through the data pointer
			rng := base.RangeFromFilename(path)
			filenameChan <- CacheFileInfo{Type: cacheType, Path: path, Range: rng, Data: data}
		}

		select {
		case <-ctx.Done():
			return ctx.Err()
		default:
		}

		return nil
	})
}

type CacheFileInfo struct {
	Type  CacheType
	Path  string
	Range base.FileRange
	IsDir bool
	Data  interface{}
}
