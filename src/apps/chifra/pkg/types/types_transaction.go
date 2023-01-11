package types

import (
	"math/big"
	"strings"

	"github.com/bykof/gostradamus"
	"github.com/ethereum/go-ethereum/common"
	"github.com/ethereum/go-ethereum/common/hexutil"
	"github.com/ethereum/go-ethereum/params"
)

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

func (s *SimpleTransaction) SetRaw(rawTransaction RawTransaction) {
	s.raw = &rawTransaction
}

// TODO: This can be made a utility
func weiToEther(wei *big.Int) *big.Float {
	return new(big.Float).Quo(new(big.Float).SetInt(wei), big.NewFloat(params.Ether))
}

func (s *SimpleTransaction) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	to := hexutil.Encode(s.To.Bytes())
	if to == "0x0000000000000000000000000000000000000000" {
		to = "0x0" // weird special case to preserve what RPC does
	}

	model := map[string]interface{}{
		"blockNumber":      s.BlockNumber,
		"transactionIndex": s.TransactionIndex,
		"timestamp":        s.Timestamp,
		"from":             s.From,
		"to":               to,
		"gasUsed":          s.GasUsed,
		"hash":             s.Hash,
		"isError":          s.IsError,
		// TODO: creates a dependacy of being in a block, so may need to be calculated on the fly, but we may
		// TODO: want to remove `finalized` as described in issue #2667
		"finalized":        extraOptions["finalized"],
	}

	// TODO: these date-related values could be done when RPC is queried and cached
	date := gostradamus.FromUnixTimestamp(int64(s.Timestamp))
	if format == "json" {
		model["blockHash"] = s.BlockHash
		model["nonce"] = s.Nonce
		model["value"] = s.Value
		model["gas"] = s.Gas
		// TODO: this value could be created when RPC is queried and cached
		model["ether"] = weiToEther(&s.Value)
		model["gasPrice"] = s.GasPrice
		model["maxFeePerGas"] = s.MaxPriorityFeePerGas
		model["maxPriorityFeePerGas"] = s.MaxPriorityFeePerGas
		model["input"] = s.Input
		model["hasToken"] = s.HasToken
		model["receipt"] = s.Receipt
		model["value"] = s.Value.String()
		model["receipt"] = nil
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

		return Model{
			Data: model,
		}
	}

	// TODO: add missing
	// "ethGasPrice": s.EthGasPrice,
	// "encoding": s.Encoding,
	// "compressedTx": s.CompressedTx,

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
	}

	return Model{
		Data:  model,
		Order: order,
	}
}
