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
	"fmt"
	"io"
	"path/filepath"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cacheNew"
)

var PendingPeriod = (5 * time.Minute).Milliseconds()

type BlockTransaction interface {
	string | SimpleTransaction
}

// EXISTING_CODE

type RawBlock struct {
	Author           string `json:"author"`
	BaseFeePerGas    string `json:"baseFeePerGas"`
	BlockNumber      string `json:"number"`
	Difficulty       string `json:"difficulty"`
	ExtraData        string `json:"extraData"`
	GasLimit         string `json:"gasLimit"`
	GasUsed          string `json:"gasUsed"`
	Hash             string `json:"hash"`
	LogsBloom        string `json:"logsBloom"`
	Miner            string `json:"miner"`
	MixHash          string `json:"mixHash"`
	Nonce            string `json:"nonce"`
	ParentHash       string `json:"parentHash"`
	ReceiptsRoot     string `json:"receiptsRoot"`
	Sha3Uncles       string `json:"sha3Uncles"`
	Size             string `json:"size"`
	StateRoot        string `json:"stateRoot"`
	Timestamp        string `json:"timestamp"`
	TotalDifficulty  string `json:"totalDifficulty"`
	Transactions     []any  `json:"transactions"`
	TransactionsRoot string `json:"transactionsRoot"`
	// EXISTING_CODE
	Uncles []string `json:"uncles"`
	// EXISTING_CODE
}

type SimpleBlock[Tx BlockTransaction] struct {
	BaseFeePerGas base.Wei       `json:"baseFeePerGas"`
	BlockNumber   base.Blknum    `json:"blockNumber"`
	Difficulty    uint64         `json:"difficulty"`
	GasLimit      base.Gas       `json:"gasLimit"`
	GasUsed       base.Gas       `json:"gasUsed"`
	Hash          base.Hash      `json:"hash"`
	Miner         base.Address   `json:"miner"`
	ParentHash    base.Hash      `json:"parentHash"`
	Timestamp     base.Timestamp `json:"timestamp"`
	Transactions  []Tx           `json:"transactions"`
	raw           *RawBlock      `json:"-"`
	// EXISTING_CODE
	// Used to be Finalized which has since been removed. Until we implement IsBackLevel
	// and upgrading cache items, this exists. We can remove it once we do so.
	UnusedBool bool        `json:"-"`
	Uncles     []base.Hash `json:"uncles,omitempty"`
	// EXISTING_CODE
}

func (s *SimpleBlock[Tx]) Raw() *RawBlock {
	return s.raw
}

func (s *SimpleBlock[Tx]) SetRaw(raw *RawBlock) {
	s.raw = raw
}

func (s *SimpleBlock[Tx]) Model(verbose bool, format string, extraOptions map[string]any) Model {
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
	}

	order = []string{
		"blockNumber",
		"timestamp",
		"hash",
		"parentHash",
		"miner",
		"difficulty",
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
					items = append(items, txObject.Model(verbose, format, extraOptions).Data)
				}
				model["transactions"] = items
			} else {
				model["transactions"] = s.Transactions
			}
			order = append(order, "transactions")
			if s.Uncles == nil {
				model["uncles"] = []base.Hash{}
			} else {
				model["uncles"] = s.Uncles
			}
			order = append(order, "uncles")
		}
	} else {
		model["transactionsCnt"] = len(s.Transactions)
		order = append(order, "transactionsCnt")
		if extraOptions["list"] == true {
			model["unclesCnt"] = len(s.Uncles)
			order = append(order, "unclesCnt")
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
func (s *SimpleBlock[Tx]) CacheName() string {
	return "Block"
}

func (s *SimpleBlock[Tx]) CacheId() string {
	return fmt.Sprintf("%09d", s.BlockNumber)
}

func (s *SimpleBlock[Tx]) CacheLocation() (directory string, extension string) {
	extension = "bin"

	paddedBn := s.CacheId()

	parts := make([]string, 3)
	parts[0] = paddedBn[:2]
	parts[1] = paddedBn[2:4]
	parts[2] = paddedBn[4:6]
	directory = filepath.Join("blocks", filepath.Join(parts...))
	return
}

func (s *SimpleBlock[Tx]) MarshalCache(writer io.Writer) (err error) {
	if err = cacheNew.WriteValue(writer, s.GasLimit); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, s.GasUsed); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, s.Hash); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, s.BlockNumber); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, s.ParentHash); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, s.Miner); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, s.Difficulty); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, s.Timestamp); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, &s.BaseFeePerGas); err != nil {
		return err
	}

	// We only cache transaction hashes (not transaction structs), so if
	// s.Transactions is []SimpleTransaction, we have to extract them
	var txHashes []string
	switch v := any(s.Transactions).(type) {
	case []string:
		txHashes = v
	case []SimpleTransaction:
		txHashes = make([]string, 0, len(s.Transactions))
		for _, tx := range v {
			txHashes = append(txHashes, tx.Hash.Hex())
		}
	}
	if err = cacheNew.WriteValue(writer, txHashes); err != nil {
		return err
	}

	if err = cacheNew.WriteValue(writer, s.Uncles); err != nil {
		return err
	}

	return
}

func (s *SimpleBlock[string]) UnmarshalCache(version uint64, reader io.Reader) (err error) {
	if err = cacheNew.ReadValue(reader, &s.GasLimit, version); err != nil {
		return err
	}
	if err = cacheNew.ReadValue(reader, &s.GasUsed, version); err != nil {
		return err
	}
	if err = cacheNew.ReadValue(reader, &s.Hash, version); err != nil {
		return err
	}
	if err = cacheNew.ReadValue(reader, &s.BlockNumber, version); err != nil {
		return err
	}
	if err = cacheNew.ReadValue(reader, &s.ParentHash, version); err != nil {
		return err
	}
	if err = cacheNew.ReadValue(reader, &s.Miner, version); err != nil {
		return err
	}
	if err = cacheNew.ReadValue(reader, &s.Difficulty, version); err != nil {
		return err
	}
	if err = cacheNew.ReadValue(reader, &s.Timestamp, version); err != nil {
		return err
	}
	if err = cacheNew.ReadValue(reader, &s.BaseFeePerGas, version); err != nil {
		return err
	}

	s.Transactions = make([]string, 0)
	if err = cacheNew.ReadValue(reader, &s.Transactions, version); err != nil {
		return err
	}

	s.Uncles = make([]base.Hash, 0)
	if err = cacheNew.ReadValue(reader, &s.Uncles, version); err != nil {
		return err
	}

	return
}

// Dup duplicates all fields but Transactions into target
func (s *SimpleBlock[string]) Dup(target *SimpleBlock[SimpleTransaction]) {
	target.BaseFeePerGas = s.BaseFeePerGas
	target.BlockNumber = s.BlockNumber
	target.Difficulty = s.Difficulty
	target.GasLimit = s.GasLimit
	target.GasUsed = s.GasUsed
	target.Hash = s.Hash
	target.Miner = s.Miner
	target.ParentHash = s.ParentHash
	target.Timestamp = s.Timestamp
	target.Uncles = s.Uncles
	target.raw = s.raw
}

func (s *SimpleBlock[Tx]) Pending(latestTimestamp int64) bool {
	// TODO: This should be 28 blocks like we use in scraper and it should be less after 1559
	return (latestTimestamp - s.Timestamp) <= PendingPeriod
}

// EXISTING_CODE
