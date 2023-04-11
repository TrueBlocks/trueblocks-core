// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package whenPkg

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"

// TODO: BOGUS2 - MUST DOCUMENT
type simpleTimestampCount struct {
	Count uint64 `json:"count"`
}

func (s *simpleTimestampCount) Raw() *types.RawModeler {
	return nil
}

func (s *simpleTimestampCount) Model(showHidden bool, format string, extraOptions map[string]any) types.Model {
	return types.Model{
		Data: map[string]interface{}{
			"count": s.Count,
		},
		Order: []string{
			"count",
		},
	}
}
