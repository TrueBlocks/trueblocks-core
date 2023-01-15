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

type RawReceipt struct {
	BlockHash         string   `json:"blockHash"`
	BlockNumber       string   `json:"blockNumber"`
	ContractAddress   string   `json:"contractAddress,omitempty"`
	CumulativeGasUsed string   `json:"cumulativeGasUsed,omitempty"`
	EffectiveGasPrice string   `json:"effectiveGasPrice"`
	From              string   `json:"from"`
	GasUsed           string   `json:"gasUsed"`
	Logs              []RawLog `json:"logs"`
	Status            string   `json:"status"`
	To                string   `json:"to"`
	TransactionHash   string   `json:"transactionHash"`
	TransactionIndex  string   `json:"transactionIndex"`
	IsError           string   `json:"isError,omitempty"`
	// Type           string   `json:"type,omitempty"`
	// Root           string   `json:"root"`
	// LogsBloom      string   `json:"logsBloom"`
}

type SimpleReceipt struct {
	BlockHash         common.Hash    `json:"blockHash"`
	BlockNumber       uint64         `json:"blockNumber"`
	ContractAddress   common.Address `json:"contractAddress,omitempty"`
	CumulativeGasUsed string         `json:"cumulativeGasUsed,omitempty"`
	From              common.Address `json:"from"`
	GasUsed           Gas            `json:"gasUsed"`
	EffectiveGasPrice Gas            `json:"effectiveGasPrice"`
	Logs              []SimpleLog    `json:"logs"`
	Status            uint32         `json:"status"`
	IsError           bool           `json:"isError,omitempty"`
	To                common.Address `json:"to"`
	TransactionHash   common.Hash    `json:"transactionHash"`
	TransactionIndex  uint64         `json:"transactionIndex"`
	raw               *RawReceipt
	// Hash           common.Hash    `json:"-"`
	// Root           common.Hash    `json:"-"`
	// LogsBloom      string         `json:"-"`
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
		"blockNumber":      s.BlockNumber,
		"transactionIndex": s.TransactionIndex,
		"transactionHash":  s.TransactionHash,
		"gasUsed":          s.GasUsed,
		"status":           s.Status,
	}

	order := []string{
		"blockNumber",
		"transactionIndex",
		"transactionHash",
		"gasUsed",
		"status",
	}

	// EXISTING_CODE
	if format == "json" {
		model["logs"] = s.Logs
		order = append(order, "logs")

		if len(s.ContractAddress) > 0 && s.ContractAddress != common.HexToAddress("0x0") {
			model["contractAddress"] = s.ContractAddress
			order = append(order, "contractAddress")
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

			if len(s.From) > 0 && s.From != common.HexToAddress("0x0") {
				model["from"] = s.From
			}

			if len(s.To) > 0 && s.To != common.HexToAddress("0x0") {
				model["to"] = s.To
			}
		}
	} else {
		model["isError"] = s.IsError
		order = append(order, "isError")

		if showHidden {
			if len(s.ContractAddress) > 0 && s.ContractAddress == common.HexToAddress("0x0") {
				model["contractAddress"] = "0x"
			} else {
				model["contractAddress"] = hexutil.Encode(s.ContractAddress.Bytes())
			}
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
