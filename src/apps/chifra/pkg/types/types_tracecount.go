// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package types

// EXISTING_CODE
import "github.com/ethereum/go-ethereum/common"

// EXISTING_CODE

type RawTraceCount struct {
	BlockNumber      string `json:"blockNumber"`
	TransactionIndex string `json:"transactionIndex"`
	TransactionHash  string `json:"transactionHash"`
	Timestamp        string `json:"timestamp"`
	TracesCnt        string `json:"tracesCnt"`
}

type SimpleTraceCount struct {
	BlockNumber      uint64      `json:"blockNumber"`
	TransactionIndex uint64      `json:"transactionIndex"`
	TransactionHash  common.Hash `json:"transactionHash"`
	Timestamp        int64       `json:"timestamp"`
	TracesCnt        uint64      `json:"tracesCnt"`
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
		"transactionIndex": s.TransactionIndex,
		"transactionHash":  s.TransactionHash,
		"timestamp":        s.Timestamp,
		"tracesCnt":        s.TracesCnt,
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
