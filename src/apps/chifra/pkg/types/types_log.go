package types

import "github.com/ethereum/go-ethereum/common"

// These types are used to unmarshal responses directly from node. We want
// to allow users to get node output directly if they want to. In other
// cases, we want to transform the data so that it is easier to work with
// in the codebase and makes more sense where printing.

type RawLog struct {
	Address          string   `json:"address"`
	Topics           []string `json:"topics"`
	Data             string   `json:"data"`
	BlockNumber      string   `json:"blockNumber"`
	TransactionHash  string   `json:"transactionHash"`
	TransactionIndex string   `json:"transactionIndex"`
	BlockHash        string   `json:"blockHash"`
	LogIndex         string   `json:"logIndex"`
	Removed          bool     `json:"removed"`
}

type SimpleLog struct {
	Address          common.Address  `json:"address"`
	LogIndex         uint64          `json:"logIndex"`
	BlockNumber      uint64          `json:"blockNumber"`
	TransactionIndex uint32          `json:"transactionIndex"`
	Timestamp        uint64          `json:"timestamp,omitempty"`
	Topics           []common.Hash   `json:"topics,omitempty"`
	Data             string          `json:"data,omitempty"`
	CompressedLog    string          `json:"compressedLog,omitempty"`
	ArticulatedLog   *SimpleFunction `json:"-"`
}
