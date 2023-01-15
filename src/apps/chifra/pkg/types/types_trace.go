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

type RawTrace struct {
	BlockHash        string         `json:"blockHash"`
	BlockNumber      string         `json:"blockNumber"`
	Timestamp        string         `json:"timestamp"`
	Subtraces        string         `json:"subtraces"`
	TraceAddress     []string       `json:"traceAddress"`
	TransactionHash  string         `json:"transactionHash"`
	TransactionIndex string         `json:"transactionIndex"`
	TraceType        string         `json:"traceType"`
	Error            string         `json:"error"`
	Action           RawTraceAction `json:"action"`
	Result           RawTraceResult `json:"result"`
}

type SimpleTrace struct {
	BlockHash        common.Hash        `json:"blockHash"`
	BlockNumber      Blknum             `json:"blockNumber"`
	Timestamp        int64              `json:"timestamp"`
	Subtraces        uint64             `json:"subtraces"`
	TraceAddress     []string           `json:"traceAddress"`
	TransactionHash  common.Hash        `json:"transactionHash"`
	TransactionIndex Blknum             `json:"transactionIndex"`
	TraceType        string             `json:"traceType"`
	Error            string             `json:"error"`
	Action           *SimpleTraceAction `json:"action"`
	Result           *SimpleTraceResult `json:"result"`
	ArticulatedTrace *SimpleFunction    `json:"articulatedTrace"`
	raw              *RawTrace
}

func (s *SimpleTrace) Raw() *RawTrace {
	return s.raw
}

func (s *SimpleTrace) SetRaw(raw *RawTrace) {
	s.raw = raw
}

func (s *SimpleTrace) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	// EXISTING_CODE
	// EXISTING_CODE

	model := map[string]interface{}{
		"error":            s.Error,
		"blockHash":        s.BlockHash,
		"blockNumber":      s.BlockNumber,
		"timestamp":        s.Timestamp,
		"transactionHash":  s.TransactionHash,
		"transactionIndex": s.TransactionIndex,
		"traceAddress":     s.TraceAddress,
		"subtraces":        s.Subtraces,
		"type":             s.TraceType,
		"action":           s.Action,
		"result":           s.Result,
		"articulatedTrace": s.ArticulatedTrace,
		// "compressedTrace":  s.CompressedTrace,
	}

	order := []string{
		"error",
		"blockHash",
		"blockNumber",
		"timestamp",
		"transactionHash",
		"transactionIndex",
		"traceAddress",
		"subtraces",
		"type",
		"action",
		"result",
		"articulatedTrace",
		"compressedTrace",
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
