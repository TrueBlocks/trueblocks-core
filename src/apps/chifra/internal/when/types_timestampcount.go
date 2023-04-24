// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package whenPkg

// EXISTING_CODE
import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"

// EXISTING_CODE

type simpleTimestampCount struct {
	Count uint64 `json:"count"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *simpleTimestampCount) Raw() *types.RawModeler {
	return nil
}

func (s *simpleTimestampCount) Model(verbose bool, format string, extraOptions map[string]any) types.Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]interface{}{
		"count": s.Count,
	}
	order = []string{
		"count",
	}
	// EXISTING_CODE

	return types.Model{
		Data:  model,
		Order: order,
	}
}

// EXISTING_CODE
// EXISTING_CODE
