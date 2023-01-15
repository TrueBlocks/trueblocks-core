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

type RawTraceResult struct {
	NewContract string `json:"newContract"`
	Code        string `json:"code"`
	GasUsed     string `json:"gasUsed"`
	Output      string `json:"output"`
}

type SimpleTraceResult struct {
	NewContract common.Address `json:"newContract"`
	Code        string         `json:"code"`
	GasUsed     Gas            `json:"gasUsed"`
	Output      string         `json:"output"`
	raw         *RawTraceResult
}

func (s *SimpleTraceResult) Raw() *RawTraceResult {
	return s.raw
}

func (s *SimpleTraceResult) SetRaw(raw *RawTraceResult) {
	s.raw = raw
}

func (s *SimpleTraceResult) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	// EXISTING_CODE
	// EXISTING_CODE

	model := map[string]interface{}{
		"newContract": s.NewContract,
		"code":        s.Code,
		"gasUsed":     s.GasUsed,
		"output":      s.Output,
	}

	order := []string{
		"newContract",
		"code",
		"gasUsed",
		"output",
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
