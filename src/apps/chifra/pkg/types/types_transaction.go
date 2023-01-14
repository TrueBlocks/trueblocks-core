package types

import "github.com/ethereum/go-ethereum/common"

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
}
