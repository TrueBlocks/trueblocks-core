package types

import (
	"github.com/ethereum/go-ethereum/common"
	"github.com/ethereum/go-ethereum/common/hexutil"
)

type RawBlock struct {
	Author           string        `json:"author,omitempty"`
	Difficulty       string        `json:"difficulty"`
	ExtraData        string        `json:"extraData,omitempty"`
	GasLimit         string        `json:"gasLimit"`
	GasUsed          string        `json:"gasUsed"`
	Hash             string        `json:"hash"`
	LogsBloom        string        `json:"logsBloom,omitempty"`
	Miner            string        `json:"miner"`
	MixHash          string        `json:"mixHash"`
	Nonce            string        `json:"nonce"`
	Number           string        `json:"number"`
	ParentHash       string        `json:"parentHash"`
	ReceiptsRoot     string        `json:"receiptsRoot"`
	SealFields       []string      `json:"sealFields"`
	Sha3Uncles       string        `json:"sha3Uncles"`
	Size             string        `json:"size"`
	StateRoot        string        `json:"stateRoot"`
	Timestamp        string        `json:"timestamp"`
	TransactionsRoot string        `json:"transactionsRoot"`
	Transactions     []interface{} `json:"transactions"`
}

type SimpleBlock struct {
	GasLimit      uint64              `json:"gasLimit"`
	GasUsed       uint64              `json:"gasUsed"`
	Hash          common.Hash         `json:"hash"`
	BlockNumber   Blknum              `json:"blockNumber"`
	ParentHash    common.Hash         `json:"parentHash"`
	Miner         common.Address      `json:"miner"`
	Difficulty    uint64              `json:"difficulty"`
	Finalized     bool                `json:"finalized"`
	Timestamp     int64               `json:"timestamp"`
	BaseFeePerGas Wei                 `json:"baseFeePerGas"`
	Transactions  []SimpleTransaction `json:"transactions"`
	raw           *RawBlock
}

func (s *SimpleBlock) Raw() *RawBlock {
	return s.raw
}

func (s *SimpleBlock) SetRaw(rawBlock RawBlock) {
	s.raw = &rawBlock
}

func (s *SimpleBlock) Model(showHidden bool, format string) Model {
	model := map[string]interface{}{
		"hash":            s.Hash,
		"blockNumber":     s.BlockNumber,
		"timestamp":       s.Timestamp,
		"difficulty":      s.Difficulty,
		"miner":           hexutil.Encode(s.Miner.Bytes()),
		"transactionsCnt": 12,
		"uncle_count":     13,
		"gasLimit":        s.GasLimit,
		"gasUsed":         s.GasUsed,
	}

	order := []string{
		"gasLimit",
		"gasUsed",
		"hash",
		"blockNumber",
		"parentHash",
		"miner",
		"difficulty",
		"timestamp",
		"baseFeePerGas",
		// "transactionsCnt",
		// "uncle_count",
	}

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *SimpleBlock) GetTimestamp() uint64 {
	return uint64(s.Timestamp)
}
