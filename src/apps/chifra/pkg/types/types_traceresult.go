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

type RawTraceResult interface{}

type SimpleTraceResult struct {
	NewContract address `json:"newContract"`
	Code        bytes   `json:"code"`
	GasUsed     gas     `json:"gasUsed"`
	Output      bytes   `json:"output"`
}

func (s *SimpleTraceResult) Raw() *RawTraceResult {
	return nil
}

func (s *SimpleTraceResult) Model(showHidden bool, format string, extraOptions map[string]any) Model {
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