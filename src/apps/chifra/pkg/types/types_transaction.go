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
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/bykof/gostradamus"
	"github.com/ethereum/go-ethereum/common"
	"github.com/ethereum/go-ethereum/common/hexutil"
)

// EXISTING_CODE

type RawTransaction struct {
	Hash             common.Hash `json:"hash"`
	BlockHash        common.Hash `json:"blockHash"`
	BlockNumber      Blknum      `json:"blockNumber"`
	TransactionIndex uint64      `json:"transactionIndex"`
}

type SimpleTransaction struct {
	Hash                 common.Hash     `json:"hash"`
	BlockHash            common.Hash     `json:"blockHash"`
	BlockNumber          Blknum          `json:"blockNumber"`
	TransactionIndex     uint64          `json:"transactionIndex"`
	Nonce                uint64          `json:"nonce"`
	Timestamp            int64           `json:"timestamp"`
	From                 common.Address  `json:"from"`
	To                   common.Address  `json:"to"`
	Value                Wei             `json:"value"`
	ExtraValue1          Wei             `json:"extraValue1"`
	ExtraValue2          Wei             `json:"extraValue2"`
	Gas                  Gas             `json:"gas"`
	GasPrice             Gas             `json:"gasPrice"`
	GasUsed              Gas             `json:"gasUsed"`
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
		"to":               to,
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
		"value",
	}

	// EXISTING_CODE
	if format == "json" {
		model["blockHash"] = s.BlockHash
		model["nonce"] = s.Nonce
		model["value"] = s.Value
		model["gas"] = s.Gas

		// TODO: this value could be created when RPC is queried and cached
		model["ether"] = utils.Wei_2_EtherStr(&s.Value)
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
			// TODO: this should be a utility (and used above as well). May be available in go-ethereum. We should check.
			contractAddress := hexutil.Encode(s.Receipt.ContractAddress.Bytes())
			if contractAddress == "0x0000000000000000000000000000000000000000" {
				contractAddress = "0x0"
			}
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
				logs = append(logs, map[string]any{
					"address":  strings.ToLower(log.Address.Hex()),
					"logIndex": log.LogIndex,
					"topics":   log.Topics,
					"data":     log.Data,
				})
			}
			receiptModel["logs"] = logs
			model["receipt"] = receiptModel
		}
	}

	// TODO: These fields are ignored
	// "ethGasPrice": s.EthGasPrice,
	// "encoding": s.Encoding,
	// "compressedTx": s.CompressedTx,
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// EXISTING_CODE
// EXISTING_CODE
