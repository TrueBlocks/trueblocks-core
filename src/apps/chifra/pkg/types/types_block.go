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
	"github.com/ethereum/go-ethereum/common"
	"github.com/ethereum/go-ethereum/common/hexutil"
)

type BlockTransaction interface {
	string | SimpleTransaction
}

// EXISTING_CODE

type RawBlock struct {
	Author           string   `json:"author,omitempty"`
	Difficulty       string   `json:"difficulty"`
	ExtraData        string   `json:"extraData,omitempty"`
	GasLimit         string   `json:"gasLimit"`
	GasUsed          string   `json:"gasUsed"`
	Hash             string   `json:"hash"`
	LogsBloom        string   `json:"logsBloom,omitempty"`
	Miner            string   `json:"miner"`
	MixHash          string   `json:"mixHash"`
	Nonce            string   `json:"nonce"`
	Number           string   `json:"number"`
	ParentHash       string   `json:"parentHash"`
	ReceiptsRoot     string   `json:"receiptsRoot"`
	Sha3Uncles       string   `json:"sha3Uncles"`
	Size             string   `json:"size"`
	StateRoot        string   `json:"stateRoot"`
	Timestamp        string   `json:"timestamp"`
	TransactionsRoot string   `json:"transactionsRoot"`
	TotalDifficulty  string   `json:"totalDifficulty"`
	Transactions     []any    `json:"transactions"`
	Uncles           []string `json:"uncles"`
	// SealFields       []string      `json:"sealFields"`
}

type SimpleBlock[Tx BlockTransaction] struct {
	GasLimit      uint64         `json:"gasLimit"`
	GasUsed       uint64         `json:"gasUsed"`
	Hash          common.Hash    `json:"hash"`
	BlockNumber   Blknum         `json:"blockNumber"`
	ParentHash    common.Hash    `json:"parentHash"`
	Miner         common.Address `json:"miner"`
	Difficulty    uint64         `json:"difficulty"`
	Finalized     bool           `json:"finalized"`
	Timestamp     int64          `json:"timestamp"`
	BaseFeePerGas Wei            `json:"baseFeePerGas"`
	Transactions  []Tx           `json:"transactions"`
	Uncles        []common.Hash  `json:"uncles"`
	raw           *RawBlock
}

func (s *SimpleBlock[Tx]) Raw() *RawBlock {
	return s.raw
}

func (s *SimpleBlock[Tx]) SetRaw(raw *RawBlock) {
	s.raw = raw
}

func (s *SimpleBlock[Tx]) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	// EXISTING_CODE
	if extraOptions["count"] == true {
		return Model{
			Data: map[string]interface{}{
				"blockNumber":     s.BlockNumber,
				"transactionsCnt": len(s.Transactions),
			},
			Order: []string{
				"blockNumber",
				"transactionsCnt",
			},
		}
	}

	if extraOptions["txHashes"] == true {
		txHashes := make([]string, 0, len(s.Transactions))
		// Check what type Tx is
		switch txs := any(s.Transactions).(type) {
		case []SimpleTransaction:
			for _, tx := range txs {
				txHashes = append(txHashes, tx.Hash.String())
			}
		case []string:
			txHashes = append(txHashes, txs...)
			// TODO: no error if can't cast?
		}
		return Model{
			Data: map[string]interface{}{
				"hash":        s.Hash,
				"blockNumber": s.BlockNumber,
				"finalized":   s.Finalized,
				"parentHash":  s.ParentHash,
				"timestamp":   s.Timestamp,
				"tx_hashes":   txHashes,
			},
			Order: []string{
				"hash",
				"blockNumber",
				"parentHash",
				"timestamp",
				"tx_hashes",
			},
		}
	}
	// EXISTING_CODE

	model := map[string]interface{}{
		"blockNumber":   s.BlockNumber,
		"timestamp":     s.Timestamp,
		"hash":          s.Hash,
		"parentHash":    s.ParentHash,
		"miner":         hexutil.Encode(s.Miner.Bytes()),
		"difficulty":    s.Difficulty,
		"finalized":     s.Finalized,
		"baseFeePerGas": s.BaseFeePerGas.Uint64(),
		"gasLimit":      s.GasLimit,
		"gasUsed":       s.GasUsed,
	}

	order := []string{
		"blockNumber",
		"timestamp",
		"hash",
		"parentHash",
		"miner",
		"difficulty",
		"finalized",
		"baseFeePerGas",
		"gasLimit",
		"gasUsed",
	}

	// EXISTING_CODE
	if format == "json" {
		if extraOptions["list"] == true {
			model["transactionsCnt"] = len(s.Transactions)
			model["unclesCnt"] = len(s.Uncles)
		} else {
			// If we wanted just transactions' hashes, we would return earlier. So here we know that we
			// have transactions as objects and want to load models for them to be able to display them
			txs, ok := any(s.Transactions).([]SimpleTransaction)
			if ok {
				items := make([]map[string]interface{}, 0, len(txs))
				for _, txObject := range txs {
					extraOptions["finalized"] = s.Finalized
					items = append(items, txObject.Model(showHidden, format, extraOptions).Data)
				}
				model["transactions"] = items
			} else {
				model["transactions"] = s.Transactions
			}
			order = append(order, "transactions")
			model["uncles"] = s.Uncles
			order = append(order, "uncles")
		}
	} else {
		model["transactionsCnt"] = len(s.Transactions)
		order = append(order, "transactionsCnt")
		if extraOptions["list"] == true {
			model["unclesCnt"] = len(s.Uncles)
			order = append(order, "unclesCnt")
		} else {
			model["finalized"] = s.Finalized
			order = append(order, "finalized")
		}
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// EXISTING_CODE
func (s *SimpleBlock[Tx]) GetTimestamp() uint64 {
	return uint64(s.Timestamp)
}

// EXISTING_CODE
