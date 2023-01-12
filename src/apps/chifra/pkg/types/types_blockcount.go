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

type RawBlockCount interface{}

type SimpleBlockCount struct {
	TransactionCnt uint64 `json:"transactionCnt"`
	UncleCnt uint64 `json:"uncleCnt,omitempty"`
	TraceCnt uint64 `json:"traceCnt,omitempty"`
	AddressCnt uint64 `json:"addressCnt,omitempty"`
}

func (s *SimpleBlockCount) Raw() *RawBlockCount {
	return nil
}

func (s *SimpleBlockCount) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	model := map[string]interface{}{
		"transactionCnt": s.TransactionCnt,
	}

	order := []string{
		"transactionCnt",
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