// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package walk

import (
	"context"
	"io/fs"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

type CacheType uint

const (
	Cache_NotACache CacheType = iota
	Cache_Abis
	Cache_Blocks
	Cache_Monitors
	Cache_Names
	Cache_Recons
	Cache_Slurps
	Cache_Tmp
	Cache_Traces
	Cache_Transactions
	Index_Bloom
	Index_Final
	Index_Ripe
	Index_Staging
	Index_Unripe
	Index_Maps
)

var cacheTypeToName = map[CacheType]string{
	Cache_NotACache:    "unknown",
	Cache_Abis:         "abis",
	Cache_Blocks:       "blocks",
	Cache_Monitors:     "monitors",
	Cache_Names:        "names",
	Cache_Recons:       "reconciliations",
	Cache_Slurps:       "slurps",
	Cache_Tmp:          "tmp",
	Cache_Traces:       "traces",
	Cache_Transactions: "transactions",
	Index_Bloom:        "bloom",
	Index_Final:        "index",
	Index_Ripe:         "ripe",
	Index_Staging:      "staging",
	Index_Unripe:       "unripe",
	Index_Maps:         "neighbors",
}

// CacheTypeToFolder is a map of cache types to the folder name (also, it acts as the mode in chifra status)
var CacheTypeToFolder = map[CacheType]string{
	Cache_NotACache:    "unknown",
	Cache_Abis:         "abis",
	Cache_Blocks:       "blocks",
	Cache_Monitors:     "monitors",
	Cache_Names:        "names",
	Cache_Recons:       "recons",
	Cache_Slurps:       "slurps",
	Cache_Tmp:          "tmp",
	Cache_Traces:       "traces",
	Cache_Transactions: "txs",
	Index_Bloom:        "blooms",
	Index_Final:        "finalized",
	Index_Ripe:         "ripe",
	Index_Staging:      "staging",
	Index_Unripe:       "unripe",
	Index_Maps:         "maps",
}

var cacheTypeToExt = map[CacheType]string{
	Cache_NotACache:    "unknown",
	Cache_Abis:         "json",
	Cache_Blocks:       "bin",
	Cache_Monitors:     "mon.bin",
	Cache_Names:        "bin",
	Cache_Recons:       "bin",
	Cache_Slurps:       "bin",
	Cache_Tmp:          "",
	Cache_Traces:       "bin",
	Cache_Transactions: "bin",
	Index_Bloom:        "bloom",
	Index_Final:        "bin",
	Index_Ripe:         "txt",
	Index_Staging:      "txt",
	Index_Unripe:       "txt",
	Index_Maps:         "bin",
}

func (ct CacheType) String() string {
	return cacheTypeToName[ct]
}

func IsCacheType(path string, cT CacheType, checkExt bool) bool {
	if !strings.Contains(path, CacheTypeToFolder[cT]) {
		return false
	}
	if checkExt && !strings.HasSuffix(path, cacheTypeToExt[cT]) {
		return false
	}
	return true
}

func GetRootPathFromCacheType(chain string, cacheType CacheType) string {
	switch cacheType {
	case Cache_Abis:
		fallthrough
	case Cache_Blocks:
		fallthrough
	case Cache_Monitors:
		fallthrough
	case Cache_Names:
		fallthrough
	case Cache_Recons:
		fallthrough
	case Cache_Slurps:
		fallthrough
	case Cache_Tmp:
		fallthrough
	case Cache_Traces:
		fallthrough
	case Cache_Transactions:
		return filepath.Join(config.GetPathToCache(chain), CacheTypeToFolder[cacheType]) + "/"
	case Index_Bloom:
		fallthrough
	case Index_Final:
		fallthrough
	case Index_Ripe:
		fallthrough
	case Index_Staging:
		fallthrough
	case Index_Unripe:
		fallthrough
	case Index_Maps:
		return filepath.Join(config.GetPathToIndex(chain), CacheTypeToFolder[cacheType]) + "/"
	case Cache_NotACache:
		fallthrough
	default:
		logger.Fatal("Should never happen in paths.go")
	}

	logger.Fatal("Should never happen in paths.go")
	return ""
}

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
