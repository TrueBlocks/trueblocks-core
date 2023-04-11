// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package whenPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

type simpleTimestamp struct {
	BlockNumber uint64         `json:"blockNumber"`
	Timestamp   base.Timestamp `json:"timestamp"`
	Diff        base.Timestamp `json:"diff"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *simpleTimestamp) Raw() *types.RawModeler {
	return nil
}

func (s *simpleTimestamp) Model(showHidden bool, format string, extraOptions map[string]any) types.Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]any{
		"blockNumber": s.BlockNumber,
		"timestamp":   s.Timestamp,
		"diff":        s.Diff,
	}
	order = []string{
		"blockNumber",
		"timestamp",
		"diff",
	}
	// EXISTING_CODE

	return types.Model{
		Data:  model,
		Order: order,
	}
}
