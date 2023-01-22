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
	"github.com/ethereum/go-ethereum/common"
	"github.com/ethereum/go-ethereum/common/hexutil"
)

// EXISTING_CODE

type RawTraceResult struct {
	Address     string `json:"address,omitempty"`
	Code        string `json:"code,omitempty"`
	GasUsed     string `json:"gasUsed,omitempty"`
	NewContract string `json:"newContract,omitempty"`
	Output      string `json:"output,omitempty"`
}

type SimpleTraceResult struct {
	Address     common.Address `json:"address,omitempty"`
	Code        string         `json:"code,omitempty"`
	GasUsed     Gas            `json:"gasUsed"`
	NewContract common.Address `json:"newContract,omitempty"`
	Output      string         `json:"output,omitempty"`
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
		"gasUsed": s.GasUsed,
		"output":  s.Output,
	}

	order := []string{
		"gasUsed",
		"output",
	}

	// EXISTING_CODE
	if format == "json" {
		if len(s.NewContract) > 0 && s.NewContract != common.HexToAddress("0x0") {
			model["newContract"] = s.NewContract
		}
		if len(s.Code) > 0 {
			model["code"] = s.Code
		}
	} else {
		if len(s.NewContract) > 0 && s.NewContract == common.HexToAddress("0x0") {
			model["newContract"] = "0x"
		} else {
			model["newContract"] = hexutil.Encode(s.NewContract.Bytes())
		}
		order = append(order, "newContract")
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// EXISTING_CODE
// EXISTING_CODE
