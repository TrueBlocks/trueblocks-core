// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package cache

import (
	"path"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
)

type CacheType uint

const (
	None CacheType = iota
	Index_Bloom
	Index_Final
	Index_Staging
	Index_Ripe
	Index_Unripe
	Cache_Block
	Cache_Tx
	Cache_Trace
	Cache_Monitor
	Cache_Neighbor
	Cache_Recon
)

// CachePath helps to keep track of cache paths and extensions depending on
// chunk type
type CachePath struct {
	Type      CacheType
	RootPath  string
	Subdir    string
	Extension string
}

var cacheFolders = map[CacheType]string{
	None:           "",
	Index_Bloom:    "blooms/",
	Index_Final:    "finalized/",
	Index_Staging:  "staging/",
	Index_Ripe:     "ripe/",
	Index_Unripe:   "unripe/",
	Cache_Block:    "blocks/",
	Cache_Tx:       "txs/",
	Cache_Trace:    "traces/",
	Cache_Monitor:  "monitors/",
	Cache_Neighbor: "neighbors/",
	Cache_Recon:    "recons/",
}

// New sets correct values of Subdir and Extension properties based on cacheType
func NewCachePath(chain string, cacheType CacheType) CachePath {
	var ret CachePath

	if cacheType == Index_Bloom || cacheType == Index_Final || cacheType == Index_Staging || cacheType == Index_Ripe || cacheType == Index_Unripe {
		ret.RootPath = config.GetPathToIndex(chain)
	} else {
		ret.RootPath = config.GetPathToCache(chain)
	}

	ret.Extension = ".bin"
	if cacheType == Index_Bloom {
		ret.Extension = ".bloom"
	} else if cacheType == None {
		ret.Extension = ""
	}
	ret.Subdir = cacheFolders[cacheType]
	ret.Type = cacheType
	return ret
}

// GetFullPath builds a full path from the Path type
func (cl *CachePath) GetFullPath(name string) string {
	if name == "" {
		return path.Join(cl.RootPath, cl.Subdir)
	}
	return path.Join(cl.RootPath, cl.Subdir, name+cl.Extension)
}

// String turns cachePath data (RootPath and Subdir) into a path
// and returns it as a string
func (cl *CachePath) String() string {
	return path.Join(cl.RootPath, cl.Subdir)
}
