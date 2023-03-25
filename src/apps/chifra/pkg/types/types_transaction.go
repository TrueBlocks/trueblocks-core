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
	"io"
	"math/big"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/bykof/gostradamus"
	"github.com/ethereum/go-ethereum/common/hexutil"
)

type StorageSlot struct {
	Address     base.Address `json:"address"`
	StorageKeys []base.Hash  `json:"storageKeys"`
}

// EXISTING_CODE

type RawTransaction struct {
	AccessList           []StorageSlot `json:"accessList"`
	BlockHash            string        `json:"blockHash"`
	BlockNumber          string        `json:"blockNumber"`
	ChainId              string        `json:"chainId"`
	From                 string        `json:"from"`
	Gas                  string        `json:"gas"`
	GasPrice             string        `json:"gasPrice"`
	Hash                 string        `json:"hash"`
	Input                string        `json:"input"`
	MaxFeePerGas         string        `json:"maxFeePerGas"`
	MaxPriorityFeePerGas string        `json:"maxPriorityFeePerGas"`
	Nonce                string        `json:"nonce"`
	To                   string        `json:"to"`
	TransactionIndex     string        `json:"transactionIndex"`
	TransactionType      string        `json:"type"`
	Value                string        `json:"value"`
	// EXISTING_CODE
	// R string `json:"r"`
	// S string `json:"s"`
	// V string `json:"v"`
	// EXISTING_CODE
}

type SimpleTransaction struct {
	ArticulatedTx        *SimpleFunction `json:"articulatedTx"`
	BlockHash            base.Hash       `json:"blockHash"`
	BlockNumber          base.Blknum     `json:"blockNumber"`
	CompressedTx         string          `json:"compressedTx"`
	Date                 string          `json:"date"`
	Encoding             string          `json:"encoding"`
	Ether                string          `json:"ether"`
	EtherGasPrice        base.Gas        `json:"etherGasPrice"`
	From                 base.Address    `json:"from"`
	Gas                  base.Gas        `json:"gas"`
	GasPrice             base.Gas        `json:"gasPrice"`
	GasUsed              base.Gas        `json:"gasUsed"`
	HasToken             bool            `json:"hasToken"`
	Hash                 base.Hash       `json:"hash"`
	Input                string          `json:"input"`
	IsError              bool            `json:"isError"`
	MaxFeePerGas         base.Gas        `json:"maxFeePerGas"`
	MaxPriorityFeePerGas base.Gas        `json:"maxPriorityFeePerGas"`
	Nonce                uint64          `json:"nonce"`
	Receipt              *SimpleReceipt  `json:"receipt"`
	Timestamp            base.Timestamp  `json:"timestamp"`
	To                   base.Address    `json:"to"`
	Traces               []SimpleTrace   `json:"traces"`
	TransactionIndex     base.Blknum     `json:"transactionIndex"`
	Value                base.Wei        `json:"value"`
	raw                  *RawTransaction `json:"-"`
	// EXISTING_CODE
	GasCost base.Gas `json:"gasCost"`
	Message string   `json:"-"`
	// EXISTING_CODE
}

func (s *SimpleTransaction) Raw() *RawTransaction {
	return s.raw
}

func (s *SimpleTransaction) SetRaw(raw *RawTransaction) {
	s.raw = raw
}

func (s *SimpleTransaction) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	to := hexutil.Encode(s.To.Bytes())
	if to == "0x0000000000000000000000000000000000000000" {
		to = "0x0" // weird special case to preserve what RPC does
	}

	model = map[string]interface{}{
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

	order = []string{
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

	date := gostradamus.FromUnixTimestamp(s.Timestamp)
	model["date"] = utils.FormattedDate(s.Timestamp)
	model["gasCost"] = s.SetGasCost(s.Receipt)

	// TODO: Shouldn't this use the SimpleFunction model - the answer is yes?
	var articulatedTx map[string]interface{}
	isArticulated := extraOptions["articulate"] == true && s.ArticulatedTx != nil
	if isArticulated {
		articulatedTx = map[string]interface{}{
			"name": s.ArticulatedTx.Name,
		}
		inputModels := ParametersToMap(s.ArticulatedTx.Inputs)
		if inputModels != nil {
			articulatedTx["inputs"] = inputModels
		}
		outputModels := ParametersToMap(s.ArticulatedTx.Outputs)
		if outputModels != nil {
			articulatedTx["outputs"] = outputModels
		}
		sm := s.ArticulatedTx.StateMutability
		if sm != "" && sm != "nonpayable" && sm != "view" {
			articulatedTx["stateMutability"] = sm
		}
	}

	if format == "json" {
		model["blockHash"] = s.BlockHash
		if s.Nonce > 0 {
			model["nonce"] = s.Nonce
		}
		model["value"] = s.Value.String()
		model["gas"] = s.Gas

		// TODO: this value could be created when RPC is queried and cached
		// if s.Value > 0 {
		model["ether"] = utils.WeiToEther(&s.Value).Text('f', 18)
		// }
		if s.MaxFeePerGas > 0 {
			model["maxFeePerGas"] = s.MaxFeePerGas
		}
		if s.MaxPriorityFeePerGas > 0 {
			model["maxPriorityFeePerGas"] = s.MaxPriorityFeePerGas
		}
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

		// model["receipt"] = nil
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

		if isArticulated {
			model["articulatedTx"] = articulatedTx

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

		if isArticulated {
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

func (s *SimpleTransaction) WriteTo(w io.Writer) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

func (s *SimpleTransaction) ReadFrom(r io.Reader) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

// EXISTING_CODE
func (s *SimpleTransaction) SetGasCost(receipt *SimpleReceipt) base.Gas {
	if receipt == nil {
		return 0
	}
	s.GasCost = s.GasPrice * receipt.GasUsed
	return s.GasCost
}

// EXISTING_CODE
