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
	"github.com/ethereum/go-ethereum/common/hexutil"
)

// EXISTING_CODE

type RawTraceResult struct {
	Address string `json:"address"`
	Code    string `json:"code"`
	GasUsed string `json:"gasUsed"`
	Output  string `json:"output"`
}

type SimpleTraceResult struct {
	Address Address `json:"address,omitempty"`
	Code    string  `json:"code,omitempty"`
	GasUsed Gas     `json:"gasUsed"`
	Output  string  `json:"output"`
	raw     *RawTraceResult
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
		"gasUsed": s.GasUsed,
		"output":  s.Output,
	}

	order := []string{
		"gasUsed",
		"output",
	}

	// EXISTING_CODE
	if format == "json" {
		if !s.Address.IsZero() {
			model["address"] = s.Address
			order = append(order, "address")
		}
		if extraOptions["tracesTransactionsFormat"] != true && len(s.Code) > 0 {
			model["code"] = s.Code
			order = append(order, "code")
		}
		// if len(s.Output) > 0 && s.Output != "0x" {
		// 	model["output"] = s.Output
		// 	order = append(order, "output")
		// }
	} else {
		if !s.Address.IsZero() {
			model["address"] = hexutil.Encode(s.Address.Bytes())
			order = append(order, "address")
		}
		// if len(s.Output) > 0 && s.Output != "0x" {
		// 	model["output"] = s.Output
		// 	order = append(order, "output")
		// }
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// EXISTING_CODE
// EXISTING_CODE
