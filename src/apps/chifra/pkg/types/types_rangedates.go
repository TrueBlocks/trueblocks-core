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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

// EXISTING_CODE

type RangeDates struct {
	FirstDate string         `json:"firstDate,omitempty"`
	FirstTs   base.Timestamp `json:"firstTs,omitempty"`
	LastDate  string         `json:"lastDate,omitempty"`
	LastTs    base.Timestamp `json:"lastTs,omitempty"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s RangeDates) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *RangeDates) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	var model = map[string]any{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]any{
		"firstDate": s.FirstDate,
		"firstTs":   s.FirstTs,
		"lastDate":  s.LastDate,
		"lastTs":    s.LastTs,
	}
	order = []string{
		"firstDate",
		"firstTs",
		"lastDate",
		"lastTs",
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *RangeDates) FinishUnmarshal(fileVersion uint64) {
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
func (p1 *RangeDates) Cmp(p2 RangeDates) int64 {
	return p1.FirstTs.Int64() - p2.FirstTs.Int64()
}

// EXISTING_CODE
