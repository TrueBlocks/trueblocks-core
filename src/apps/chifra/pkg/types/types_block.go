package types

import (
	"time"

	"github.com/ethereum/go-ethereum/common"
)

type RawBlock interface{}

type SimpleBlock struct {
	GasLimit      uint64              `json:"gasLimit"`
	GasUsed       uint64              `json:"gasUsed"`
	Hash          common.Hash         `json:"hash"`
	BlockNumber   Blknum              `json:"blockNumber"`
	ParentHash    common.Hash         `json:"parentHash"`
	Miner         common.Address      `json:"miner"`
	Difficulty    uint64              `json:"difficulty"`
	Finalized     bool                `json:"finalized"`
	Timestamp     time.Time           `json:"timestamp"`
	BaseFeePerGas Wei                 `json:"baseFeePerGas"`
	Transactions  []SimpleTransaction `json:"transactions"`
}

func (s *SimpleBlock) Raw() *RawBlock {
	return nil
}
