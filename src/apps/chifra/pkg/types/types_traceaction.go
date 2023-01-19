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
)

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
	Address        common.Address `json:"address,omitempty"`
	Balance        Wei            `json:"balance,omitempty"`
	CallType       string         `json:"callType,omitempty"`
	From           common.Address `json:"from,omitempty"`
	Gas            Gas            `json:"gas"`
	Init           string         `json:"init,omitempty"`
	Input          string         `json:"input,omitempty"`
	RefundAddress  common.Address `json:"refundAddress,omitempty"`
	SelfDestructed common.Address `json:"selfDestructed,omitempty"`
	To             common.Address `json:"to,omitempty"`
	Value          Wei            `json:"value"`
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
		if len(s.SelfDestructed) > 0 && s.SelfDestructed != common.HexToAddress("0x0") {
			model["selfDestructed"] = s.SelfDestructed
		}
		if s.Gas != 0 {
			model["gas"] = s.Gas
		}
		if len(s.Input) > 0 {
			model["input"] = s.Input
		}
		if len(s.RefundAddress) > 0 && s.RefundAddress != common.HexToAddress("0x0") {
			model["refundAddress"] = s.RefundAddress
			model["balance"] = s.Balance.String()
			if s.Value.String() != "0" {
				model["value"] = s.Balance.String()
			}
		} else {
			model["value"] = s.Value.String()
		}
		if len(s.Address) > 0 && s.Address != common.HexToAddress("0x0") {
			model["address"] = s.Address
		}
		if len(s.CallType) > 0 {
			model["callType"] = s.CallType
		}
		if len(s.From) > 0 && s.From != common.HexToAddress("0x0") {
			model["from"] = s.From
		}
		if len(s.To) > 0 && s.To != common.HexToAddress("0x0") {
			model["to"] = s.To
		}

		// 	txs, ok := any(s.Transactions).([]SimpleTransaction)
		// 	if ok {
		// 		items := make([]map[string]interface{}, 0, len(txs))
		// 		for _, txObject := range txs {
		// 			extraOptions["finalized"] = s.Finalized
		// 			items = append(items, txObject.Model(showHidden, format, extraOptions).Data)
		// 		}
		// 		model["transactions"] = items
		// 	} else {
		// 		model["transactions"] = s.Transactions
		// 	}
		// 	order = append(order, "transactions")
		// 	model["uncles"] = s.Uncles
		// 	order = append(order, "uncles")
	} else {
		// model["transactionsCnt"] = len(s.Transactions)
		// order = append(order, "transactionsCnt")
		// if extraOptions["list"] == true {
		// 	model["unclesCnt"] = len(s.Uncles)
		// 	order = append(order, "unclesCnt")
		// } else {
		// 	model["finalized"] = s.Finalized
		// 	order = append(order, "finalized")
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
