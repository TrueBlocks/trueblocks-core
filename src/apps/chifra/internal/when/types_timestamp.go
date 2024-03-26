// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package whenPkg

// EXISTING_CODE
import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// EXISTING_CODE

type simpleTimestamp struct {
	BlockNumber base.Blknum    `json:"blockNumber"`
	Diff        int64          `json:"diff"`
	Timestamp   base.Timestamp `json:"timestamp"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *simpleTimestamp) Raw() *types.RawModeler {
	return nil
}

func (s *simpleTimestamp) Model(chain, format string, verbose bool, extraOptions map[string]any) types.Model {
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

func (s *simpleTimestamp) Date() string {
	return utils.FormattedDate(s.Timestamp)
}

// EXISTING_CODE
// EXISTING_CODE
