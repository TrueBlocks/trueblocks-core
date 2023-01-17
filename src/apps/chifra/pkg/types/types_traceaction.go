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

type RawTraceAction struct {
	SelfDestructed string `json:"selfDestructed"`
	Balance        string `json:"balance"`
	Value          string `json:"value"`
	Init           string `json:"init"`
	From           string `json:"from"`
	To             string `json:"to"`
	Gas            string `json:"gas"`
	Input          string `json:"input"`
	CallType       string `json:"callType"`
	RefundAddress  string `json:"refundAddress"`
}

type SimpleTraceAction struct {
	SelfDestructed common.Address `json:"selfDestructed,omitempty"`
	Balance        Wei            `json:"balance,omitempty"`
	Value          Wei            `json:"value"`
	Init           string         `json:"init,omitempty"`
	From           common.Address `json:"from"`
	To             common.Address `json:"to"`
	Gas            Gas            `json:"gas"`
	Input          string         `json:"input,omitempty"`
	CallType       string         `json:"callType"`
	RefundAddress  common.Address `json:"refundAddress,omitempty"`
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

	model := map[string]interface{}{
		"value":          s.Value,
		"from":           s.From,
		"to":             s.To,
		"gas":            s.Gas,
		"callType":       s.CallType,
	}

	order := []string{
		"value",
		"from",
		"to",
		"gas",
		"callType",
	}

	// EXISTING_CODE
	// TODO: BOGUS - HANDLE OMITEMPTY FIELDS HERE
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// EXISTING_CODE
// EXISTING_CODE
