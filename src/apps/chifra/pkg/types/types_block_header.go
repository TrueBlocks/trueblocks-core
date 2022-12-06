package types

import (
	"github.com/ethereum/go-ethereum/common"
)

type SimpleBlockHeader struct {
	BlockNumber Blknum `json:"blockNumber"`
	Timestamp   int64  `json:"timestamp"`
	//
	GasLimit      uint64              `json:"gasLimit"`
	GasUsed       uint64              `json:"gasUsed"`
	Hash          common.Hash         `json:"hash"`
	ParentHash    common.Hash         `json:"parentHash"`
	Miner         common.Address      `json:"miner"`
	Difficulty    uint64              `json:"difficulty"`
	Finalized     bool                `json:"finalized"`
	BaseFeePerGas Wei                 `json:"baseFeePerGas"`
	Transactions  []SimpleTransaction `json:"transactions"`
}

func (s *SimpleBlockHeader) GetTimestamp() uint64 {
	return uint64(s.Timestamp)
}
