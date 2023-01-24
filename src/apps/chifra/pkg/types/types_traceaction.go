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

type RawTraceAction struct {
	Address        string `json:"address,omitempty"`
	From           string `json:"from,omitempty"`
	CallType       string `json:"callType,omitempty"`
	Gas            string `json:"gas,omitempty"`
	Init           string `json:"init,omitempty"`
	Input          string `json:"input,omitempty"`
	RefundAddress  string `json:"refundAddress,omitempty"`
	SelfDestructed string `json:"selfDestructed,omitempty"`
	To             string `json:"to,omitempty"`
	Value          string `json:"value,omitempty"`
	Balance        string `json:"balance,omitempty"`
}

type SimpleTraceAction struct {
	Address        Address `json:"address,omitempty"`
	Balance        Wei     `json:"balance,omitempty"`
	CallType       string  `json:"callType,omitempty"`
	From           Address `json:"from,omitempty"`
	Gas            Gas     `json:"gas"`
	Init           string  `json:"init,omitempty"`
	Input          string  `json:"input,omitempty"`
	RefundAddress  Address `json:"refundAddress,omitempty"`
	SelfDestructed Address `json:"selfDestructed,omitempty"`
	To             Address `json:"to,omitempty"`
	Value          Wei     `json:"value"`
	raw            *RawTraceAction
}

func (s *SimpleTraceAction) Raw() *RawTraceAction {
	return s.raw
}

func (s *SimpleTraceAction) SetRaw(raw *RawTraceAction) {
	s.raw = raw
}

func (s *SimpleTraceAction) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	// EXISTING_CODE
	// EXISTING_CODE

	model := map[string]interface{}{}
	order := []string{}

	// EXISTING_CODE
	if format == "json" {
		// TODO: this should be a utility (and used above as well). May be available in go-ethereum. We should check.
		if s.Value.String() != "0" {
			model["value"] = s.Value.String()
		}
		if len(s.Init) > 0 {
			model["init"] = s.Init
		}
		if s.SelfDestructed.IsZero() {
			model["selfDestructed"] = s.SelfDestructed
		}
		if s.Gas != 0 {
			model["gas"] = s.Gas
		}
		if len(s.Input) > 0 {
			model["input"] = s.Input
		}
		if s.RefundAddress.IsZero() {
			model["refundAddress"] = s.RefundAddress
			model["balance"] = s.Balance.String()
			if s.Value.String() != "0" {
				model["value"] = s.Balance.String()
			}
		} else {
			model["to"] = s.To
			model["value"] = s.Value.String()
		}
		if !s.Address.IsZero() {
			model["address"] = s.Address
		}
		if len(s.CallType) > 0 {
			model["callType"] = s.CallType
		}
		if !s.From.IsZero() {
			model["from"] = s.From
		}
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// EXISTING_CODE
// EXISTING_CODE
