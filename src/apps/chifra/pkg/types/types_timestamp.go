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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// EXISTING_CODE

type RawTimestamp struct {
	BlockNumber string `json:"blockNumber"`
	Diff        string `json:"diff"`
	Timestamp   string `json:"timestamp"`
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleTimestamp struct {
	BlockNumber base.Blknum    `json:"blockNumber"`
	Diff        int64          `json:"diff"`
	Timestamp   base.Timestamp `json:"timestamp"`
	raw         *RawTimestamp  `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimpleTimestamp) Raw() *RawTimestamp {
	return s.raw
}

func (s *SimpleTimestamp) SetRaw(raw *RawTimestamp) {
	s.raw = raw
}

func (s *SimpleTimestamp) Model(chain, format string, verbose bool, extraOptions map[string]any) Model {
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

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *SimpleTimestamp) Date() string {
	return utils.FormattedDate(s.Timestamp)
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *SimpleTimestamp) FinishUnmarshal() {
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
