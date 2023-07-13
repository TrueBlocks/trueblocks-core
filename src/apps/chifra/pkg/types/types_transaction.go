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
	"fmt"
	"io"
	"math/big"
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/binary"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
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

func (s *SimpleTransaction) Model(verbose bool, format string, extraOptions map[string]any) Model {
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
		model["ether"] = utils.FormattedValue(s.Value, true, 18)
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
				traceModels = append(traceModels, trace.Model(verbose, format, extraOptions).Data)
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
		model["ether"] = utils.FormattedValue(s.Value, true, 18)
		ethGasPrice := utils.FormattedValue(*big.NewInt(0).SetUint64(s.GasPrice), true, 18)
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

func (s *SimpleTransaction) CacheName() string {
	return "Transaction"
}

func (s *SimpleTransaction) CacheId() string {
	return fmt.Sprintf("%09d-%05d", s.BlockNumber, s.TransactionIndex)
}

func (s *SimpleTransaction) CacheLocation() (directory string, extension string) {
	extension = "bin"

	id := s.CacheId()

	parts := make([]string, 3)
	parts[0] = id[:2]
	parts[1] = id[2:4]
	parts[2] = id[4:6]
	directory = filepath.Join("txs", filepath.Join(parts...))
	return
}

func (s *SimpleTransaction) MarshalCache(writer io.Writer) (err error) {
	optArticulatedTx := &binary.Optional[SimpleFunction]{
		Value: s.ArticulatedTx,
	}
	if err = binary.WriteValue(writer, optArticulatedTx); err != nil {
		return err
	}
	if err = binary.WriteValue(writer, &s.Hash); err != nil {
		return err
	}
	if err = binary.WriteValue(writer, &s.BlockHash); err != nil {
		return err
	}
	if err = binary.WriteValue(writer, s.BlockNumber); err != nil {
		return err
	}
	if err = binary.WriteValue(writer, s.TransactionIndex); err != nil {
		return err
	}
	if err = binary.WriteValue(writer, s.Nonce); err != nil {
		return err
	}
	if err = binary.WriteValue(writer, s.Timestamp); err != nil {
		return err
	}
	if err = binary.WriteValue(writer, s.From); err != nil {
		return err
	}
	if err = binary.WriteValue(writer, s.To); err != nil {
		return err
	}
	if err = binary.WriteValue(writer, &s.Value); err != nil {
		return err
	}
	if err = binary.WriteValue(writer, s.Gas); err != nil {
		return err
	}
	if err = binary.WriteValue(writer, s.GasPrice); err != nil {
		return err
	}
	if err = binary.WriteValue(writer, s.GasUsed); err != nil {
		return err
	}
	if err = binary.WriteValue(writer, s.GasCost); err != nil {
		return err
	}
	if err = binary.WriteValue(writer, s.MaxFeePerGas); err != nil {
		return err
	}
	if err = binary.WriteValue(writer, s.MaxPriorityFeePerGas); err != nil {
		return err
	}
	if err = binary.WriteValue(writer, s.Input); err != nil {
		return err
	}
	if err = binary.WriteValue(writer, s.IsError); err != nil {
		return err
	}
	if err = binary.WriteValue(writer, s.HasToken); err != nil {
		return err
	}
	if err = binary.WriteValue(writer, s.Receipt); err != nil {
		return err
	}

	// TODO:
	// Traces

	return
}

func (s *SimpleTransaction) UnmarshalCache(version uint64, reader io.Reader) (err error) {
	optArticulatedTx := &binary.Optional[SimpleFunction]{
		Value: s.ArticulatedTx,
	}
	if err = binary.ReadValue(reader, optArticulatedTx, version); err != nil {
		return err
	}
	s.ArticulatedTx = optArticulatedTx.Get()

	if err = binary.ReadValue(reader, &s.Hash, version); err != nil {
		return err
	}
	if err = binary.ReadValue(reader, &s.BlockHash, version); err != nil {
		return err
	}
	if err = binary.ReadValue(reader, &s.BlockNumber, version); err != nil {
		return err
	}
	if err = binary.ReadValue(reader, &s.TransactionIndex, version); err != nil {
		return err
	}
	if err = binary.ReadValue(reader, &s.Nonce, version); err != nil {
		return err
	}
	if err = binary.ReadValue(reader, &s.Timestamp, version); err != nil {
		return err
	}
	if err = binary.ReadValue(reader, &s.From, version); err != nil {
		return err
	}
	if err = binary.ReadValue(reader, &s.To, version); err != nil {
		return err
	}
	if err = binary.ReadValue(reader, &s.Value, version); err != nil {
		return err
	}
	if err = binary.ReadValue(reader, &s.Gas, version); err != nil {
		return err
	}
	if err = binary.ReadValue(reader, &s.GasPrice, version); err != nil {
		return err
	}
	if err = binary.ReadValue(reader, &s.GasUsed, version); err != nil {
		return err
	}
	if err = binary.ReadValue(reader, &s.GasCost, version); err != nil {
		return err
	}
	if err = binary.ReadValue(reader, &s.MaxFeePerGas, version); err != nil {
		return err
	}
	if err = binary.ReadValue(reader, &s.MaxPriorityFeePerGas, version); err != nil {
		return err
	}
	if err = binary.ReadValue(reader, &s.Input, version); err != nil {
		return err
	}
	if err = binary.ReadValue(reader, &s.IsError, version); err != nil {
		return err
	}
	if err = binary.ReadValue(reader, &s.HasToken, version); err != nil {
		return err
	}

	s.Receipt = &SimpleReceipt{}
	if err = binary.ReadValue(reader, s.Receipt, version); err != nil {
		return err
	}

	s.Date = utils.FormattedDate(s.Timestamp)

	// TODO:
	// Traces

	return
}

// EXISTING_CODE
