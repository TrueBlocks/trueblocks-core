package types

import (
	"time"

	"github.com/ethereum/go-ethereum/common"
)

// TODO: Will need to spell this out when we implement --raw
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

func (s *SimpleBlock) Model(showHidden bool, format string) Model {
	model := map[string]interface{}{
		"hash":            s.Hash,
		"blockNumber":     s.BlockNumber,
		"timestamp":       s.Timestamp,
		"difficulty":      s.Difficulty,
		"miner":           s.Miner,
		"transactionsCnt": 12,
		"uncle_count":     13,
		"gasLimit":        s.GasLimit,
		"gasUsed":         s.GasUsed,
	}

	order := []string{
		"hash",
		"blockNumber",
		"timestamp",
		"difficulty",
		"miner",
		"transactionsCnt",
		"uncle_count",
		"gasLimit",
		"gasUsed",
	}

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *SimpleBlock) GetTimestamp() uint64 {
	return uint64(s.Timestamp.Unix())
}
