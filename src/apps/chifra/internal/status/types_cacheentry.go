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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// EXISTING_CODE

type simpleCacheEntry struct {
	Address        base.Address `json:"address"`
	Cached         bool         `json:"cached"`
	Extra          string       `json:"extra"`
	Name           string       `json:"name"`
	Path           string       `json:"path"`
	CacheEntryType uint64       `json:"type"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *simpleCacheEntry) Raw() *types.RawModeler {
	return nil
}

func (s *simpleCacheEntry) Model(showHidden bool, format string, extraOptions map[string]any) types.Model {
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
