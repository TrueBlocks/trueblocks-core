// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package statusPkg

// EXISTING_CODE
import (
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// EXISTING_CODE

type simpleCacheItem struct {
	Items         []any  `json:"items"`
	LastCached    string `json:"lastCached,omitempty"`
	NFiles        uint64 `json:"nFiles"`
	NFolders      uint64 `json:"nFolders"`
	Path          string `json:"path"`
	SizeInBytes   int64  `json:"sizeInBytes"`
	CacheItemType string `json:"type"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *simpleCacheItem) Raw() *types.RawModeler {
	return nil
}

func (s *simpleCacheItem) Model(verbose bool, format string, extraOptions map[string]any) types.Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	// EXISTING_CODE

	return types.Model{
		Data:  model,
		Order: order,
	}
}

// EXISTING_CODE
//

func NewSingleCacheStats(t cache.CacheType, now time.Time) *simpleCacheItem {
	return &simpleCacheItem{
		CacheItemType: t.CacheName(),
		Items:         make([]any, 0),
		LastCached:    now.Format("2006-01-02 15:04:05"),
		NFiles:        0,
		NFolders:      0,
		Path:          "",
		SizeInBytes:   0,
	}
}

// EXISTING_CODE
