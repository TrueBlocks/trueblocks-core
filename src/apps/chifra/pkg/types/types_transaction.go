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
	"math/big"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/bykof/gostradamus"
	"github.com/ethereum/go-ethereum/common"
	"github.com/ethereum/go-ethereum/common/hexutil"
)

// EXISTING_CODE

type RawTransaction struct {
	BlockHash        string `json:"blockHash"`
	BlockNumber      string `json:"blockNumber"`
	ChainId          string `json:"chainId,omitempty"`
	From             string `json:"from"`
	Gas              string `json:"gas"`
	GasPrice         string `json:"gasPrice"`
	Hash             string `json:"hash"`
	Input            string `json:"input"`
	Nonce            string `json:"nonce"`
	R                string `json:"r"`
	S                string `json:"s"`
	To               string `json:"to"`
	TransactionIndex string `json:"transactionIndex"`
	Type             string `json:"type"`
	V                string `json:"v"`
	Value            string `json:"value"`
}

type SimpleTransaction struct {
	Hash                 common.Hash     `json:"hash"`
	BlockHash            common.Hash     `json:"blockHash"`
	BlockNumber          Blknum          `json:"blockNumber"`
	TransactionIndex     uint64          `json:"transactionIndex"`
	Nonce                uint64          `json:"nonce"`
	Timestamp            int64           `json:"timestamp"`
	From                 Address         `json:"from"`
	To                   Address         `json:"to"`
	Value                Wei             `json:"value"`
	ExtraValue1          Wei             `json:"extraValue1"`
	ExtraValue2          Wei             `json:"extraValue2"`
	Gas                  Gas             `json:"gas"`
	GasPrice             Gas             `json:"gasPrice"`
	GasUsed              Gas             `json:"gasUsed"`
	GasCost              Gas             `json:"gasCost"`
	MaxFeePerGas         Gas             `json:"maxFeePerGas"`
	MaxPriorityFeePerGas Gas             `json:"maxPriorityFeePerGas"`
	Input                string          `json:"input"`
	IsError              bool            `json:"isError,omitempty"`
	HasToken             bool            `json:"hasToken,omitempty"`
	Cachebits            uint8           `json:"cachebits"`
	Reserved2            uint8           `json:"reserved2"`
	Receipt              *SimpleReceipt  `json:"receipt"`
	Traces               []SimpleTrace   `json:"traces"`
	ArticulatedTx        *SimpleFunction `json:"articulatedTx,omitempty"`
	Message              string          `json:"-"`
	raw                  *RawTransaction
}

func (s *SimpleTransaction) Raw() *RawTransaction {
	return s.raw
}

func (s *SimpleTransaction) SetRaw(raw *RawTransaction) {
	s.raw = raw
}

func (s *SimpleTransaction) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	// EXISTING_CODE
	to := hexutil.Encode(s.To.Bytes())
	if to == "0x0000000000000000000000000000000000000000" {
		to = "0x0" // weird special case to preserve what RPC does
	}

	date := gostradamus.FromUnixTimestamp(int64(s.Timestamp))
	// EXISTING_CODE

	model := map[string]interface{}{
		"blockNumber":      s.BlockNumber,
		"from":             s.From,
		"gasPrice":         s.GasPrice,
		"gasUsed":          s.GasUsed,
		"hash":             s.Hash,
		"timestamp":        s.Timestamp,
		"to":               to,
		"transactionIndex": s.TransactionIndex,
		"value":            s.Value.String(),
	}

	order := []string{
		"blockNumber",
		"transactionIndex",
		"date",
		"timestamp",
		"from",
		"to",
		"ether",
		"gasPrice",
		"gasUsed",
		"gasCost",
		"hash",
		"isError",
		"encoding",
		"compressedTx",
	}

	// EXISTING_CODE
	model["date"] = date.Format("2006-01-02 15:04:05") + " UTC"
	model["gasCost"] = s.SetGasCost(s.Receipt)

	if format == "json" {
		model["blockHash"] = s.BlockHash
		model["nonce"] = s.Nonce
		model["value"] = s.Value.String()
		model["gas"] = s.Gas

		// TODO: this value could be created when RPC is queried and cached
		model["ether"] = utils.WeiToEther(&s.Value)
		model["maxFeePerGas"] = s.MaxPriorityFeePerGas
		model["maxPriorityFeePerGas"] = s.MaxPriorityFeePerGas
		model["input"] = s.Input
		if s.HasToken {
			model["hasToken"] = s.HasToken
		}
		if s.IsError {
			model["isError"] = s.IsError
		}

		if showHidden {
			model["datesh"] = date.Format("2006-01-02")
			model["time"] = date.Format("15:04:05") + " UTC"
		}

		model["receipt"] = nil
		if s.Receipt != nil {
			contractAddress := s.Receipt.ContractAddress.Hex()

			// TODO: this should not be hardcoded here. We have tslib.GetSpecials(), but there
			// TODO: are 2 issues with it: 1. circular dependency with types package, 2. every
			// TODO: call to GetSpecials parses CSV file, so we need to call it once and cache
			byzantiumBlock := uint64(4370000)
			status := &s.Receipt.Status
			if s.BlockNumber < byzantiumBlock || *status == 4294967295-1 {
				status = nil
			}

			receiptModel := map[string]any{
				"contractAddress":   contractAddress,
				"effectiveGasPrice": s.Receipt.EffectiveGasPrice,
				"gasUsed":           s.Receipt.GasUsed,
				"status":            status,
			}

			// TODO: We've already made a copy of the data that we've queried from the chain,
			// TODO: why are we copying it yet again? Can't we use pointers to the one copy of the data?
			logs := make([]map[string]any, 0, len(s.Receipt.Logs))
			for _, log := range s.Receipt.Logs {
				logModel := map[string]any{
					"address":  log.Address.Hex(),
					"logIndex": log.LogIndex,
					"topics":   log.Topics,
					"data":     log.Data,
				}
				if extraOptions["articulate"] == true && log.ArticulatedLog != nil {
					inputModels := ParametersToMap(log.ArticulatedLog.Inputs)
					articulatedLog := map[string]any{
						"name":   log.ArticulatedLog.Name,
						"inputs": inputModels,
					}
					logModel["articulatedLog"] = articulatedLog
				}
				logs = append(logs, logModel)
			}
			receiptModel["logs"] = logs
			model["receipt"] = receiptModel
		}

		if extraOptions["traces"] == true && len(s.Traces) > 0 {
			traceModels := make([]map[string]any, 0, len(s.Traces))
			for _, trace := range s.Traces {
				traceModels = append(traceModels, trace.Model(showHidden, format, extraOptions).Data)
			}
			model["traces"] = traceModels
		}

		if extraOptions["articulate"] == true && s.ArticulatedTx != nil {
			inputModels := ParametersToMap(s.ArticulatedTx.Inputs)
			outputModels := ParametersToMap(s.ArticulatedTx.Outputs)
			// TODO: Shouldn't this be a SimpleFunction?
			articulatedTx := map[string]any{
				"name":    s.ArticulatedTx.Name,
				"inputs":  inputModels,
				"outputs": outputModels,
			}
			if s.ArticulatedTx.StateMutability != "" && s.ArticulatedTx.StateMutability != "nonpayable" {
				articulatedTx["stateMutability"] = s.ArticulatedTx.StateMutability
			}
			if format == "json" {
				model["articulatedTx"] = articulatedTx
			}
		} else {
			if s.Message != "" {
				model["message"] = s.Message
			}
		}

	} else {
		etherValue := utils.WeiToEther(&s.Value).Text('f', 18)
		model["ether"] = etherValue
		ethGasPrice := utils.WeiToEther(big.NewInt(0).SetUint64(s.GasPrice)).Text('f', 18)
		model["ethGasPrice"] = ethGasPrice
		model["isError"] = s.IsError

		if extraOptions["articulate"] == true && s.ArticulatedTx != nil {
			model["encoding"] = s.ArticulatedTx.Encoding
		}

		model["compressedTx"] = ""
		enc := s.Input
		if len(s.Input) >= 10 {
			enc = s.Input[:10]
		}
		model["encoding"] = enc

		if extraOptions["articulate"] == true && s.ArticulatedTx != nil {
			inputModels := ParametersToMap(s.ArticulatedTx.Inputs)
			outputModels := ParametersToMap(s.ArticulatedTx.Outputs)
			// TODO: Shouldn't this be a SimpleFunction?
			articulatedTx := map[string]any{
				"name":    s.ArticulatedTx.Name,
				"inputs":  inputModels,
				"outputs": outputModels,
			}
			if s.ArticulatedTx.StateMutability != "" && s.ArticulatedTx.StateMutability != "nonpayable" {
				articulatedTx["stateMutability"] = s.ArticulatedTx.StateMutability
			}
			if format == "json" {
				model["articulatedTx"] = articulatedTx
			}
			model["compressedTx"] = MakeCompressed(articulatedTx)
		} else if s.Message != "" {
			model["encoding"] = ""
			model["compressedTx"] = s.Message
		}

		if extraOptions["traces"] == true {
			model["nTraces"] = len(s.Traces)
			order = append(order, "nTraces")
		}
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// EXISTING_CODE
func (s *SimpleTransaction) SetGasCost(receipt *SimpleReceipt) Gas {
	s.GasCost = s.GasPrice * receipt.GasUsed
	return s.GasCost
}

// EXISTING_CODE
