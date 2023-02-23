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

type RawReceipt struct {
	From              string   `json:"from"`
	To                string   `json:"to"`
	EffectiveGasPrice string   `json:"effectiveGasPrice"`
	CumulativeGasUsed string   `json:"cumulativeGasUsed"`
	BlockNumber       string   `json:"blockNumber"`
	TransactionIndex  string   `json:"transactionIndex"`
	TransactionHash   string   `json:"transactionHash"`
	BlockHash         string   `json:"blockHash"`
	Status            string   `json:"status"`
	GasUsed           string   `json:"gasUsed"`
	ContractAddress   string   `json:"contractAddress"`
	IsError           string   `json:"isError"`
	Logs              []RawLog `json:"logs"`
}

type SimpleReceipt struct {
	From              Address     `json:"from,omitempty"`
	To                Address     `json:"to,omitempty"`
	EffectiveGasPrice Gas         `json:"effectiveGasPrice,omitempty"`
	CumulativeGasUsed string      `json:"cumulativeGasUsed,omitempty"`
	BlockNumber       uint64      `json:"blockNumber"`
	TransactionIndex  uint64      `json:"transactionIndex"`
	TransactionHash   common.Hash `json:"transactionHash"`
	BlockHash         common.Hash `json:"blockHash,omitempty"`
	Status            uint32      `json:"status"`
	GasUsed           Gas         `json:"gasUsed"`
	ContractAddress   Address     `json:"contractAddress,omitempty"`
	IsError           bool        `json:"isError,omitempty"`
	Logs              []SimpleLog `json:"logs"`
	raw               *RawReceipt
}

func (s *SimpleReceipt) Raw() *RawReceipt {
	return s.raw
}

func (s *SimpleReceipt) SetRaw(raw *RawReceipt) {
	s.raw = raw
}

func (s *SimpleReceipt) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	// EXISTING_CODE
	// EXISTING_CODE

	model := map[string]interface{}{
		"blockNumber":       s.BlockNumber,
		"transactionIndex":  s.TransactionIndex,
		"transactionHash":   s.TransactionHash,
		"status":            s.Status,
		"gasUsed":           s.GasUsed,
	}

	order := []string{
		"blockNumber",
		"transactionIndex",
		"transactionHash",
		"status",
		"gasUsed",
	}

	// EXISTING_CODE
	if format == "json" {
		model["logs"] = s.Logs
		order = append(order, "logs")

		if len(s.ContractAddress.Bytes()) > 0 && s.ContractAddress != HexToAddress("0x0") {
			model["contractAddress"] = s.ContractAddress
		}

		if s.IsError {
			model["isError"] = s.IsError
			order = append(order, "isError")
		}

		if showHidden {
			model["blockHash"] = s.BlockHash
			order = append(order, "blockHash")

			model["cumulativeGasUsed"] = s.CumulativeGasUsed
			order = append(order, "cumulativeGasUsed")

			if !s.From.IsZero() {
				model["from"] = s.From
			}

			if !s.To.IsZero() {
				model["to"] = s.To
			}
		}
	} else {
		model["logsCnt"] = len(s.Logs)
		order = append(order, "logsCnt")

		model["isError"] = s.IsError
		order = append(order, "isError")

		if showHidden {
			model["contractAddress"] = s.ContractAddress.Hex()
			order = append(order, "contractAddress")
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
