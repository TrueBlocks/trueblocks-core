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

type RawTraceAction interface{}

type SimpleTraceAction struct {
	SelfDestructed address `json:"selfDestructed,omitempty"`
	Balance        wei     `json:"balance,omitempty"`
	Value          wei     `json:"value"`
	Init           string  `json:"init"`
	From           address `json:"from"`
	To             address `json:"to"`
	Gas            gas     `json:"gas"`
	Input          bytes   `json:"input"`
	CallType       string  `json:"callType"`
	RefundAddress  address `json:"refundAddress"`
}

func (s *SimpleTraceAction) Raw() *RawTraceAction {
	return nil
}

func (s *SimpleTraceAction) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	model := map[string]interface{}{
		"value":          s.Value,
		"init":           s.Init,
		"from":           s.From,
		"to":             s.To,
		"gas":            s.Gas,
		"input":          s.Input,
		"callType":       s.CallType,
		"refundAddress":  s.RefundAddress,
	}

	order := []string{
		"value",
		"init",
		"from",
		"to",
		"gas",
		"input",
		"callType",
		"refundAddress",
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