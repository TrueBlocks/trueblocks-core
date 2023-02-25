package types

import "github.com/ethereum/go-ethereum/common"

// TODO: Enable auto code generation here
// These types are used to unmarshal responses directly from node. We want
// to allow users to get node output directly if they want to. In other
// cases, we want to transform the data so that it is easier to work with
// in the codebase and makes more sense where printing.

type RawLog struct {
	Address          string   `json:"address"`
	BlockHash        string   `json:"blockHash"`
	BlockNumber      string   `json:"blockNumber"`
	Data             string   `json:"data"`
	LogIndex         string   `json:"logIndex"`
	Removed          bool     `json:"removed"`
	Topics           []string `json:"topics"`
	TransactionHash  string   `json:"transactionHash"`
	TransactionIndex string   `json:"transactionIndex"`
}

type SimpleLog struct {
	Address          Address         `json:"address"`
	ArticulatedLog   *SimpleFunction `json:"-"`
	BlockNumber      uint64          `json:"blockNumber"`
	CompressedLog    string          `json:"compressedLog,omitempty"`
	Data             string          `json:"data,omitempty"`
	LogIndex         uint64          `json:"logIndex"`
	Timestamp        uint64          `json:"timestamp,omitempty"`
	Topics           []common.Hash   `json:"topics,omitempty"`
	TransactionIndex uint32          `json:"transactionIndex"`
}
