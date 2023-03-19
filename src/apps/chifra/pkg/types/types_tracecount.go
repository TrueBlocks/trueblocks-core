// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package types

// EXISTING_CODE
import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/ethereum/go-ethereum/common"
)

// EXISTING_CODE

type RawTraceCount struct {
	BlockNumber      string `json:"blockNumber"`
	Timestamp        string `json:"timestamp"`
	TracesCnt        string `json:"tracesCnt"`
	TransactionHash  string `json:"transactionHash"`
	TransactionIndex string `json:"transactionIndex"`
}

type SimpleTraceCount struct {
	BlockNumber      base.Blknum    `json:"blockNumber"`
	Timestamp        base.Timestamp `json:"timestamp"`
	TracesCnt        uint64         `json:"tracesCnt"`
	TransactionHash  common.Hash    `json:"transactionHash"`
	TransactionIndex base.Blknum    `json:"transactionIndex"`
	raw              *RawTraceCount
}

func (s *SimpleTraceCount) Raw() *RawTraceCount {
	return s.raw
}

func (s *SimpleTraceCount) SetRaw(raw *RawTraceCount) {
	s.raw = raw
}

func (s *SimpleTraceCount) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	// EXISTING_CODE
	// EXISTING_CODE

	model := map[string]interface{}{
		"blockNumber":      s.BlockNumber,
		"timestamp":        s.Timestamp,
		"tracesCnt":        s.TracesCnt,
		"transactionHash":  s.TransactionHash,
		"transactionIndex": s.TransactionIndex,
	}

	order := []string{
		"blockNumber",
		"transactionIndex",
		"transactionHash",
		"timestamp",
		"tracesCnt",
	}

	// EXISTING_CODE
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// EXISTING_CODE
// EXISTING_CODE
