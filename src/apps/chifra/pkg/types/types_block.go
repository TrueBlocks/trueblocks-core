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
	"io"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

type BlockTransaction interface {
	string | SimpleTransaction
}

// EXISTING_CODE

type RawBlock struct {
	Author           string   `json:"author,omitempty"`    // address
	BaseFeePerGas    string   `json:"baseFeePerGas"`       // gas
	Difficulty       string   `json:"difficulty"`          // int
	ExtraData        string   `json:"extraData,omitempty"` // byte data
	GasLimit         string   `json:"gasLimit"`            // gas
	GasUsed          string   `json:"gasUsed"`             // gas
	Hash             string   `json:"hash"`                // hash
	LogsBloom        string   `json:"logsBloom,omitempty"` // byte data
	Miner            string   `json:"miner"`               // address
	MixHash          string   `json:"mixHash"`             // hash
	Nonce            string   `json:"nonce"`               // hash
	Number           string   `json:"number"`              // block number
	ParentHash       string   `json:"parentHash"`          // hash
	ReceiptsRoot     string   `json:"receiptsRoot"`        // hash
	SealFields       []string `json:"sealFields"`          // array of hashes
	Sha3Uncles       string   `json:"sha3Uncles"`          // hash
	Size             string   `json:"size"`                // value
	StateRoot        string   `json:"stateRoot"`           // hash
	Timestamp        string   `json:"timestamp"`           // timestamp
	TotalDifficulty  string   `json:"totalDifficulty"`     // value
	Transactions     []any    `json:"transactions"`        // array of transactions or hash
	TransactionsRoot string   `json:"transactionsRoot"`    // hash
	Uncles           []string `json:"uncles"`              // array of hashes
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleBlock[Tx BlockTransaction] struct {
	BaseFeePerGas base.Wei       `json:"baseFeePerGas"`
	BlockNumber   base.Blknum    `json:"blockNumber"`
	Difficulty    uint64         `json:"difficulty"`
	Finalized     bool           `json:"finalized"`
	GasLimit      base.Gas       `json:"gasLimit"`
	GasUsed       base.Gas       `json:"gasUsed"`
	Hash          base.Hash      `json:"hash"`
	Miner         base.Address   `json:"miner"`
	ParentHash    base.Hash      `json:"parentHash"`
	Timestamp     base.Timestamp `json:"timestamp"`
	Transactions  []Tx           `json:"transactions"`
	Uncles        []base.Hash    `json:"uncles"`
	raw           *RawBlock      `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimpleBlock[Tx]) Raw() *RawBlock {
	return s.raw
}

func (s *SimpleBlock[Tx]) SetRaw(raw *RawBlock) {
	s.raw = raw
}

func (s *SimpleBlock[Tx]) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

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

	if extraOptions["hashes"] == true {
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

	model = map[string]interface{}{
		"gasUsed":       s.GasUsed,
		"gasLimit":      s.GasLimit,
		"hash":          s.Hash,
		"blockNumber":   s.BlockNumber,
		"parentHash":    s.ParentHash,
		"miner":         s.Miner.Hex(),
		"difficulty":    s.Difficulty,
		"timestamp":     s.Timestamp,
		"baseFeePerGas": s.BaseFeePerGas.Uint64(),
		"finalized":     s.Finalized,
	}

	order = []string{
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

func (s *SimpleBlock[Tx]) WriteTo(w io.Writer) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

func (s *SimpleBlock[Tx]) ReadFrom(r io.Reader) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

// EXISTING_CODE
// EXISTING_CODE
