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
)

// EXISTING_CODE

type Bounds struct {
	Count     uint64         `json:"count"`
	FirstApp  Appearance     `json:"firstApp"`
	FirstTs   base.Timestamp `json:"firstTs"`
	LatestApp Appearance     `json:"latestApp"`
	LatestTs  base.Timestamp `json:"latestTs"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s Bounds) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *Bounds) Raw() *Bounds {
	return s
}

func (s *Bounds) SetRaw(raw *Bounds) {
}

func (s *Bounds) Model(chain, format string, verbose bool, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	extraOptions["appearances"] = true
	model = map[string]interface{}{
		"address":    s.FirstApp.Address,
		"count":      s.Count,
		"firstApp":   s.FirstApp.Model(chain, format, verbose, extraOptions).Data, //fmt.Sprintf("%d.%d", s.FirstApp.BlockNumber, s.FirstApp.TransactionIndex),
		"firstTs":    s.FirstTs,
		"firstDate":  base.FormattedDate(s.FirstTs),
		"latestApp":  s.LatestApp.Model(chain, format, verbose, extraOptions).Data, //fmt.Sprintf("%d.%d", s.LatestApp.BlockNumber, s.LatestApp.TransactionIndex),
		"latestTs":   s.LatestTs,
		"latestDate": base.FormattedDate(s.LatestTs),
		"blockSpan":  (s.LatestApp.BlockNumber - s.FirstApp.BlockNumber),
		"blockFreq":  uint64(s.LatestApp.BlockNumber-s.FirstApp.BlockNumber) / s.Count,
	}
	if format == "txt" || format == "csv" {
		model["firstApp"] = fmt.Sprintf("%d.%d", s.FirstApp.BlockNumber, s.FirstApp.TransactionIndex)
		model["latestApp"] = fmt.Sprintf("%d.%d", s.LatestApp.BlockNumber, s.LatestApp.TransactionIndex)
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
