// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package statusPkg

// EXISTING_CODE
import (
	"encoding/json"

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

func (s *simpleCacheItem) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *simpleCacheItem) Raw() *types.RawModeler {
	return nil
}

func (s *simpleCacheItem) Model(chain, format string, verbose bool, extraOptions map[string]any) types.Model {
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
// EXISTING_CODE
