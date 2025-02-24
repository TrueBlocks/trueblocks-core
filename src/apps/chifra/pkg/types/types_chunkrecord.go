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

type ChunkRecord struct {
	BloomHash  base.IpfsHash `json:"bloomHash"`
	BloomSize  int64         `json:"bloomSize"`
	IndexHash  base.IpfsHash `json:"indexHash"`
	IndexSize  int64         `json:"indexSize"`
	Range      string        `json:"range"`
	RangeDates *RangeDates   `json:"rangeDates,omitempty"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s ChunkRecord) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *ChunkRecord) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	_ = chain
	_ = format
	_ = verbose
	_ = extraOpts
	var model = map[string]any{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]any{
		"range":     s.Range,
		"bloomHash": s.BloomHash,
		"bloomSize": s.BloomSize,
		"indexHash": s.IndexHash,
		"indexSize": s.IndexSize,
	}
	order = []string{
		"range",
		"bloomHash",
		"bloomSize",
		"indexHash",
		"indexSize",
	}

	if verbose && format == "json" {
		if s.RangeDates != nil {
			model["rangeDates"] = s.RangeDates.Model(chain, format, verbose, extraOpts).Data
		}
	} else if verbose {
		model["firstTs"], model["firstDate"], model["lastTs"], model["lastDate"] = 0, "", 0, ""
		order = append(order, []string{"firstTs", "firstDate", "lastTs", "lastDate"}...)
		if s.RangeDates != nil {
			model["firstTs"] = s.RangeDates.FirstTs
			model["firstDate"] = s.RangeDates.FirstDate
			model["lastTs"] = s.RangeDates.LastTs
			model["lastDate"] = s.RangeDates.LastDate
		}
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *ChunkRecord) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
