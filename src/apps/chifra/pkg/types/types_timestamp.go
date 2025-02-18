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

type Timestamp struct {
	BlockNumber base.Blknum    `json:"blockNumber"`
	Diff        int64          `json:"diff"`
	Timestamp   base.Timestamp `json:"timestamp"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s Timestamp) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *Timestamp) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	var model = map[string]any{}
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

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *Timestamp) Date() string {
	return base.FormattedDate(s.Timestamp)
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *Timestamp) FinishUnmarshal(fileVersion uint64) {
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
