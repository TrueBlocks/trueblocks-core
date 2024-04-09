// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package tracesPkg

// EXISTING_CODE
import (
	"encoding/json"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// EXISTING_CODE

type simpleTraceCount struct {
	BlockNumber      base.Blknum    `json:"blockNumber"`
	Timestamp        base.Timestamp `json:"timestamp"`
	TracesCnt        uint64         `json:"tracesCnt"`
	TransactionHash  base.Hash      `json:"transactionHash"`
	TransactionIndex base.Blknum    `json:"transactionIndex"`

	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *simpleTraceCount) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *simpleTraceCount) Raw() *types.RawModeler {
	return nil
}

func (s *simpleTraceCount) Model(chain, format string, verbose bool, extraOptions map[string]any) types.Model {
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

	return types.Model{
		Data:  model,
		Order: order,
	}
}

func (s *simpleTraceCount) Date() string {
	return utils.FormattedDate(s.Timestamp)
}

// EXISTING_CODE
// EXISTING_CODE
