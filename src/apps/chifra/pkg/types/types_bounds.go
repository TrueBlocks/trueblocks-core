// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package types

// EXISTING_CODE
import (
	"encoding/json"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// EXISTING_CODE

type RawBounds struct {
	Count     string `json:"count"`
	FirstApp  string `json:"firstApp"`
	FirstTs   string `json:"firstTs"`
	LatestApp string `json:"latestApp"`
	LatestTs  string `json:"latestTs"`
	// EXISTING_CODE
	// EXISTING_CODE
}

type Bounds struct {
	Count     uint64         `json:"count"`
	FirstApp  RawAppearance  `json:"firstApp"`
	FirstTs   base.Timestamp `json:"firstTs"`
	LatestApp RawAppearance  `json:"latestApp"`
	LatestTs  base.Timestamp `json:"latestTs"`
	raw       *RawBounds     `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s Bounds) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *Bounds) Raw() *RawBounds {
	return s.raw
}

func (s *Bounds) SetRaw(raw *RawBounds) {
	s.raw = raw
}

func (s *Bounds) Model(chain, format string, verbose bool, extraOptions map[string]any) Model {
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

	return Model{
		Data:  model,
		Order: order,
	}
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *Bounds) FinishUnmarshal() {
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
