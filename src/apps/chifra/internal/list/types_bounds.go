// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package listPkg

// EXISTING_CODE
import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// EXISTING_CODE

type simpleBounds struct {
	Count      uint64              `json:"count"`
	FirstApp   types.RawAppearance `json:"firstApp"`
	FirstDate  string              `json:"firstDate"`
	FirstTs    base.Timestamp      `json:"firstTs"`
	LatestApp  types.RawAppearance `json:"latestApp"`
	LatestDate string              `json:"latestDate"`
	LatestTs   base.Timestamp      `json:"latestTs"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *simpleBounds) Raw() *types.RawModeler {
	return nil
}

func (s *simpleBounds) Model(verbose bool, format string, extraOptions map[string]any) types.Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]interface{}{
		"address":    s.FirstApp.Address,
		"count":      s.Count,
		"firstApp":   fmt.Sprintf("%d.%d", s.FirstApp.BlockNumber, s.FirstApp.TransactionIndex),
		"firstTs":    s.FirstTs,
		"firstDate":  utils.FormattedDate(s.FirstTs),
		"latestApp":  fmt.Sprintf("%d.%d", s.LatestApp.BlockNumber, s.LatestApp.TransactionIndex),
		"latestTs":   s.LatestTs,
		"latestDate": utils.FormattedDate(s.LatestTs),
		"blockSpan":  (s.LatestApp.BlockNumber - s.FirstApp.BlockNumber),
		"blockFreq":  uint64(s.LatestApp.BlockNumber-s.FirstApp.BlockNumber) / s.Count,
	}
	order = []string{
		"address",
		"count",
		"firstApp",
		"firstTs",
		"firstDate",
		"latestApp",
		"latestTs",
		"latestDate",
		"blockSpan",
		"blockFreq",
	}
	// EXISTING_CODE

	return types.Model{
		Data:  model,
		Order: order,
	}
}

// EXISTING_CODE
// EXISTING_CODE
