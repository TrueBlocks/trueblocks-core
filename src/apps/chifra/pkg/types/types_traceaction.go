// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package types

// EXISTING_CODE
import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"

// EXISTING_CODE

type RawTraceAction struct {
	Address        string `json:"address,omitempty"`
	Balance        string `json:"balance,omitempty"`
	CallType       string `json:"callType,omitempty"`
	From           string `json:"from,omitempty"`
	Gas            string `json:"gas,omitempty"`
	Init           string `json:"init,omitempty"`
	Input          string `json:"input,omitempty"`
	RefundAddress  string `json:"refundAddress,omitempty"`
	SelfDestructed string `json:"selfDestructed,omitempty"`
	To             string `json:"to,omitempty"`
	Value          string `json:"value,omitempty"`
}

type SimpleTraceAction struct {
	Address        base.Address `json:"address,omitempty"`
	Balance        base.Wei     `json:"balance,omitempty"`
	CallType       string       `json:"callType"`
	From           base.Address `json:"from"`
	Gas            base.Gas     `json:"gas"`
	Init           string       `json:"init,omitempty"`
	Input          string       `json:"input,omitempty"`
	RefundAddress  base.Address `json:"refundAddress,omitempty"`
	SelfDestructed base.Address `json:"selfDestructed,omitempty"`
	To             base.Address `json:"to"`
	Value          base.Wei     `json:"value"`
	raw            *RawTraceAction
}

func (s *SimpleTraceAction) Raw() *RawTraceAction {
	return s.raw
}

func (s *SimpleTraceAction) SetRaw(raw *RawTraceAction) {
	s.raw = raw
}

func (s *SimpleTraceAction) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	if format == "json" {
		if extraOptions["traces"] != true && len(s.Init) > 0 {
			model["init"] = s.Init
		}
		if !s.SelfDestructed.IsZero() {
			model["selfDestructed"] = s.SelfDestructed
		}
		if !s.From.IsZero() {
			model["from"] = s.From
		}
		if len(s.CallType) > 0 {
			model["callType"] = s.CallType
		}
		if s.Gas != 0 {
			model["gas"] = s.Gas
		}
		if len(s.Input) > 0 {
			model["input"] = s.Input
		}
		if s.Value.String() != "0" {
			model["value"] = s.Value.String()
		}
		if !s.RefundAddress.IsZero() {
			model["refundAddress"] = s.RefundAddress
			model["balance"] = s.Balance.String()
			if s.Value.String() != "0" {
				model["value"] = s.Balance.String()
			}
		} else {
			if s.To.IsZero() {
				model["to"] = "0x0"
			} else {
				model["to"] = s.To
			}
			model["value"] = s.Value.String()
		}
		if len(s.Init) > 0 {
			model["init"] = s.Init
		}
		if !s.Address.IsZero() {
			model["address"] = s.Address
		}
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *SimpleTraceAction) Write(p []byte) (n int, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

func (s *SimpleTraceAction) Read(p []byte) (n int, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

// EXISTING_CODE
// EXISTING_CODE
