// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package types

// EXISTING_CODE
// EXISTING_CODE

type RawBlockCount struct {
	BlockNumber     string `json:"blockNumber"`
	Timestamp       string `json:"timestamp"`
	TransactionsCnt string `json:"transactionsCnt"`
	UnclesCnt       string `json:"unclesCnt"`
	LogsCnt         string `json:"logsCnt"`
	TracesCnt       string `json:"tracesCnt"`
	AppsCnt         string `json:"appsCnt"`
	UniqsCnt        string `json:"uniqsCnt"`
}

type SimpleBlockCount struct {
	BlockNumber     uint64 `json:"blockNumber"`
	Timestamp       int64  `json:"timestamp"`
	TransactionsCnt uint64 `json:"transactionsCnt"`
	UnclesCnt       uint64 `json:"unclesCnt,omitempty"`
	LogsCnt         uint64 `json:"logsCnt,omitempty"`
	TracesCnt       uint64 `json:"tracesCnt,omitempty"`
	AppsCnt         uint64 `json:"appsCnt,omitempty"`
	UniqsCnt        uint64 `json:"uniqsCnt,omitempty"`
	raw             *RawBlockCount
}

func (s *SimpleBlockCount) Raw() *RawBlockCount {
	return s.raw
}

func (s *SimpleBlockCount) SetRaw(raw *RawBlockCount) {
	s.raw = raw
}

func (s *SimpleBlockCount) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	// EXISTING_CODE
	// EXISTING_CODE

	model := map[string]interface{}{
		"blockNumber":     s.BlockNumber,
		"timestamp":       s.Timestamp,
		"transactionsCnt": s.TransactionsCnt,
	}

	order := []string{
		"blockNumber",
		"timestamp",
		"transactionsCnt",
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
