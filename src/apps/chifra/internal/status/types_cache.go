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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// EXISTING_CODE

type simpleCache struct {
	Status simpleStatus      `json:"status,omitempty"`
	Caches []simpleCacheItem `json:"caches,omitempty"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *simpleCache) Raw() *types.RawModeler {
	return nil
}

func (s *simpleCache) Model(showHidden bool, format string, extraOptions map[string]any) types.Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	testMode := extraOptions["testMode"] == true
	statusModel := s.Status.Model(showHidden, format, extraOptions)
	if testMode {
		for i := 0; i < len(s.Caches); i++ {
			s.Caches[i].Path = "--paths--"
			s.Caches[i].LastCached = "--lastCached--"
			s.Caches[i].NFiles = 123
			s.Caches[i].NFolders = 456
			s.Caches[i].SizeInBytes = 789
		}
	}
	statusModel.Data["caches"] = s.Caches
	statusModel.Order = append(statusModel.Order, "caches")
	if showHidden {
		if !testMode {
			statusModel.Data["chains"] = config.GetRootConfig().Chains
		}
		statusModel.Order = append(statusModel.Order, "chains")
	}

	model = statusModel.Data
	order = statusModel.Order
	// EXISTING_CODE

	return types.Model{
		Data:  model,
		Order: order,
	}
}

// EXISTING_CODE
// TODO: BOGUS2 - MUST DOCUMENT
type simpleCacheItem struct {
	CacheType   string `json:"type"`
	Items       []any  `json:"items"`
	LastCached  string `json:"lastCached,omitempty"`
	NFiles      uint64 `json:"nFiles"`
	NFolders    uint64 `json:"nFolders"`
	Path        string `json:"path"`
	SizeInBytes int64  `json:"sizeInBytes"`
}

func NewSingleCacheStats(t cache.CacheType, now time.Time) *simpleCacheItem {
	return &simpleCacheItem{
		CacheType:   t.CacheName(),
		Items:       make([]any, 0),
		LastCached:  now.Format("2006-01-02 15:04:05"),
		NFiles:      0,
		NFolders:    0,
		Path:        "",
		SizeInBytes: 0,
	}
}

// EXISTING_CODE
