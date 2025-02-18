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

type MonitorClean struct {
	Address  base.Address `json:"address"`
	Dups     int64        `json:"dups"`
	Removed  bool         `json:"removed"`
	SizeNow  int64        `json:"sizeNow"`
	SizeThen int64        `json:"sizeThen"`
	Staged   bool         `json:"staged"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s MonitorClean) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *MonitorClean) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	var model = map[string]any{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]any{
		"address":  s.Address,
		"sizeNow":  s.SizeNow,
		"sizeThen": s.SizeThen,
		"dups":     s.Dups,
	}
	order = []string{
		"address",
		"sizeNow",
		"sizeThen",
		"dups",
	}

	if extraOpts["staged"] == true {
		model["staged"] = s.Staged
		model["removed"] = s.Removed
		order = append(order, "staged", "removed")
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *MonitorClean) FinishUnmarshal(fileVersion uint64) {
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
