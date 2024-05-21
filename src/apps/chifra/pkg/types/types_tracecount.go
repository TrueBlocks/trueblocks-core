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

type TraceCount struct {
	BlockNumber      base.Blknum    `json:"blockNumber"`
	Timestamp        base.Timestamp `json:"timestamp"`
	TracesCnt        uint64         `json:"tracesCnt"`
	TransactionHash  base.Hash      `json:"transactionHash"`
	TransactionIndex base.Txnum     `json:"transactionIndex"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s TraceCount) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *TraceCount) Model(chain, format string, verbose bool, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]interface{}{
		"blockNumber":      s.BlockNumber,
		"timestamp":        s.Timestamp,
		"tracesCnt":        s.TracesCnt,
		"transactionHash":  s.TransactionHash,
		"transactionIndex": s.TransactionIndex,
	}
	order = []string{
		"blockNumber",
		"transactionIndex",
		"transactionHash",
		"timestamp",
		"tracesCnt",
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *TraceCount) Date() string {
	return base.FormattedDate(s.Timestamp)
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *TraceCount) FinishUnmarshal() {
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
