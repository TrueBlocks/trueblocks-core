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
)

// EXISTING_CODE

type RawTransaction struct {
	Hash             string `json:"hash"`
	BlockHash        string `json:"blockHash"`
	BlockNumber      string `json:"blockNumber"`
	TransactionIndex string `json:"transactionIndex"`
	From             string `json:"from"`
	Gas              string `json:"gas"`
	GasPrice         string `json:"gasPrice"`
	Input            string `json:"input"`
	Nonce            string `json:"nonce"`
	To               string `json:"to"`
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
	IsError              bool            `json:"isError"`
	HasToken             bool            `json:"hasToken"`
	Cachebits            uint8           `json:"cachebits"`
	Reserved2            uint8           `json:"reserved2"`
	Receipt              *SimpleReceipt  `json:"receipt"`
	Traces               []SimpleTrace   `json:"traces"`
	ArticulatedTx        *SimpleFunction `json:"articulatedTx"`
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
	// TODO: these date-related values could be done when RPC is queried and cached
	date := gostradamus.FromUnixTimestamp(int64(s.Timestamp))

	// TODO: Imporant note. The `finalized` field creates a dependacy on the transaction model that
	// TODO: it is in a block (or at least we know the blockNumber. Also, who's to say what `finalized`
	// TODO: means? Also, `finalized` has a meaning in post-merge code. See #2667
	// EXISTING_CODE

	model := map[string]interface{}{
		"blockNumber":      s.BlockNumber,
		"transactionIndex": s.TransactionIndex,
		"timestamp":        s.Timestamp,
		"from":             s.From,
		"to":               s.To,
		"gasUsed":          s.GasUsed,
		"hash":             s.Hash,
		"isError":          s.IsError,
		"finalized":        extraOptions["finalized"],
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
		"ethGasPrice",
		"gasUsed",
		"hash",
		"isError",
		"encoding",
		"compressedTx",
		// "value",
	}

	if format != "json" {
		// TODO: these date-related values could be done when RPC is queried and cached
		date := gostradamus.FromUnixTimestamp(int64(s.Timestamp))
		model["date"] = date.Format("2006-01-02 15:04:05") + " UTC"

		etherValue := utils.WeiToEther(&s.Value).Text('f', 18)
		model["ether"] = etherValue
		ethGasPrice := utils.WeiToEther(big.NewInt(0).SetUint64(s.GasPrice)).Text('f', 18)
		model["ethGasPrice"] = ethGasPrice

		if s.ArticulatedTx != nil {
			model["encoding"] = s.ArticulatedTx.Encoding
		}
	}

	// EXISTING_CODE
	if format == "json" {
		model["blockHash"] = s.BlockHash
		model["nonce"] = s.Nonce
		model["value"] = s.Value
		model["gas"] = s.Gas

		// TODO: this value could be created when RPC is queried and cached
		model["ether"] = utils.WeiToEther(&s.Value)
		model["gasPrice"] = s.GasPrice
		model["maxFeePerGas"] = s.MaxPriorityFeePerGas
		model["maxPriorityFeePerGas"] = s.MaxPriorityFeePerGas
		model["input"] = s.Input
		model["hasToken"] = s.HasToken
		model["receipt"] = s.Receipt
		model["value"] = s.Value.String() // TODO: Why twice?
		model["receipt"] = nil            // TODO: Why twice?

		// TODO: these date-related values could be done when RPC is queried and cached
		model["date"] = date.Format("2006-01-02 15:04:05") + " UTC"
		model["datesh"] = date.Format("2006-01-02")
		model["time"] = date.Format("15:04:05") + " UTC"

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
					logModel["compressedLog"] = MakeCompressed(articulatedLog)
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
	}

	if extraOptions["articulate"] == true {
		if s.ArticulatedTx != nil {
			inputModels := ParametersToMap(s.ArticulatedTx.Inputs)
			outputModels := ParametersToMap(s.ArticulatedTx.Outputs)
			articulated := map[string]any{
				"name":            s.ArticulatedTx.Name,
				"stateMutability": s.ArticulatedTx.StateMutability,
				"inputs":          inputModels,
				"outputs":         outputModels,
			}

			if format == "json" {
				model["articulatedTx"] = articulated
			}
			model["compressedTx"] = MakeCompressed(articulated)
		} else {
			if s.Message != "" {
				model["compressedTx"] = "message:" + s.Message
			}
		}
	}

	// TODO: These fields are ignored "ethGasPrice": s.EthGasPrice, "encoding": s.Encoding
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
