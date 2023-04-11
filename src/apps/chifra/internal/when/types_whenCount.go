// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package whenPkg

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"

// TODO: BOGUS2 - MUST DOCUMENT
type simpleWhenCount struct {
	Count uint64 `json:"count"`
}

func (s *simpleWhenCount) Raw() *types.RawModeler {
	return nil
}

func (s *simpleWhenCount) Model(showHidden bool, format string, extraOptions map[string]any) types.Model {
	return types.Model{
		Data: map[string]interface{}{
			"count": s.Count,
		},
		Order: []string{
			"count",
		},
	}
}
