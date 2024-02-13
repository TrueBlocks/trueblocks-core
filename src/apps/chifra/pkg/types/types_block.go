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
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// EXISTING_CODE

type RawBlock struct {
	Author           string          `json:"author"`
	BaseFeePerGas    string          `json:"baseFeePerGas"`
	BlockNumber      string          `json:"number"`
	Difficulty       string          `json:"difficulty"`
	ExtraData        string          `json:"extraData"`
	GasLimit         string          `json:"gasLimit"`
	GasUsed          string          `json:"gasUsed"`
	Hash             string          `json:"hash"`
	LogsBloom        string          `json:"logsBloom"`
	Miner            string          `json:"miner"`
	MixHash          string          `json:"mixHash"`
	Nonce            string          `json:"nonce"`
	ParentHash       string          `json:"parentHash"`
	ReceiptsRoot     string          `json:"receiptsRoot"`
	Sha3Uncles       string          `json:"sha3Uncles"`
	Size             string          `json:"size"`
	StateRoot        string          `json:"stateRoot"`
	Timestamp        string          `json:"timestamp"`
	TotalDifficulty  string          `json:"totalDifficulty"`
	Transactions     []any           `json:"transactions"`
	TransactionsRoot string          `json:"transactionsRoot"`
	Uncles           []string        `json:"uncles"`
	Withdrawals      []RawWithdrawal `json:"withdrawals"`
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleBlock[Tx string | SimpleTransaction] struct {
	BaseFeePerGas base.Wei           `json:"baseFeePerGas"`
	BlockNumber   base.Blknum        `json:"blockNumber"`
	Difficulty    uint64             `json:"difficulty"`
	GasLimit      base.Gas           `json:"gasLimit"`
	GasUsed       base.Gas           `json:"gasUsed"`
	Hash          base.Hash          `json:"hash"`
	Miner         base.Address       `json:"miner"`
	ParentHash    base.Hash          `json:"parentHash"`
	Timestamp     base.Timestamp     `json:"timestamp"`
	Transactions  []Tx               `json:"transactions"`
	Uncles        []base.Hash        `json:"uncles,omitempty"`
	Withdrawals   []SimpleWithdrawal `json:"withdrawals,omitempty"`
	raw           *RawBlock          `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimpleBlock[Tx]) Raw() *RawBlock {
	return s.raw
}

func (s *SimpleBlock[Tx]) SetRaw(raw *RawBlock) {
	s.raw = raw
}

func (s *SimpleBlock[Tx]) Model(chain, format string, verbose bool, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
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
		model := Model{
			Data: map[string]interface{}{
				"hash":        s.Hash,
				"blockNumber": s.BlockNumber,
				"parentHash":  s.ParentHash,
				"timestamp":   s.Timestamp,
				"date":        s.Date(),
			},
			Order: []string{
				"hash",
				"blockNumber",
				"parentHash",
				"timestamp",
				"date",
			},
		}

		if format == "json" {
			model.Data["tx_hashes"] = txHashes
			if s.BlockNumber >= base.KnownBlock(chain, base.Shanghai) {
				withs := make([]map[string]any, 0, len(s.Withdrawals))
				for _, w := range s.Withdrawals {
					withs = append(withs, w.Model(chain, format, verbose, extraOptions).Data)
				}
				model.Data["withdrawals"] = withs
			}
		} else {
			model.Data["transactionsCnt"] = len(txHashes)
			model.Order = append(model.Order, "transactionsCnt")
			if s.BlockNumber > base.KnownBlock(chain, base.Shanghai) {
				model.Data["withdrawalsCnt"] = len(s.Withdrawals)
				model.Order = append(model.Order, "withdrawalsCnt")
			}
		}

		return model
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
		"date":          s.Date(),
		"baseFeePerGas": s.BaseFeePerGas.Uint64(),
	}

	order = []string{
		"blockNumber",
		"timestamp",
		"date",
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
			model["withdrawalsCnt"] = len(s.Withdrawals)
		} else {
			// If we wanted just transactions' hashes, we would return earlier. So here we know that we
			// have transactions as objects and want to load models for them to be able to display them
			txs, ok := any(s.Transactions).([]SimpleTransaction)
			if ok {
				items := make([]map[string]interface{}, 0, len(txs))
				for _, txObject := range txs {
					items = append(items, txObject.Model(chain, format, verbose, extraOptions).Data)
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
			if len(s.Withdrawals) > 0 {
				withs := make([]map[string]any, 0, len(s.Withdrawals))
				for _, w := range s.Withdrawals {
					withs = append(withs, w.Model(chain, format, verbose, extraOptions).Data)
				}
				model["withdrawals"] = withs
				order = append(order, "withdrawals")
			} else {
				model["withdrawals"] = []SimpleWithdrawal{}
			}
		}
	} else {
		model["transactionsCnt"] = len(s.Transactions)
		order = append(order, "transactionsCnt")
		model["withdrawalsCnt"] = len(s.Withdrawals)
		order = append(order, "withdrawalsCnt")
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

func (s *SimpleBlock[Tx]) Date() string {
	return utils.FormattedDate(s.Timestamp)
}

// --> cacheable by block
func (s *SimpleBlock[Tx]) CacheName() string {
	return "Block"
}

func (s *SimpleBlock[Tx]) CacheId() string {
	return fmt.Sprintf("%09d", s.BlockNumber)
}

func (s *SimpleBlock[Tx]) CacheLocation() (directory string, extension string) {
	paddedId := s.CacheId()
	parts := make([]string, 3)
	parts[0] = paddedId[:2]
	parts[1] = paddedId[2:4]
	parts[2] = paddedId[4:6]

	subFolder := strings.ToLower(s.CacheName()) + "s"
	directory = filepath.Join(subFolder, filepath.Join(parts...))
	extension = "bin"

	return
}

func (s *SimpleBlock[Tx]) MarshalCache(writer io.Writer) (err error) {
	// BaseFeePerGas
	if err = cache.WriteValue(writer, &s.BaseFeePerGas); err != nil {
		return err
	}

	// BlockNumber
	if err = cache.WriteValue(writer, s.BlockNumber); err != nil {
		return err
	}

	// Difficulty
	if err = cache.WriteValue(writer, s.Difficulty); err != nil {
		return err
	}

	// GasLimit
	if err = cache.WriteValue(writer, s.GasLimit); err != nil {
		return err
	}

	// GasUsed
	if err = cache.WriteValue(writer, s.GasUsed); err != nil {
		return err
	}

	// Hash
	if err = cache.WriteValue(writer, &s.Hash); err != nil {
		return err
	}

	// Miner
	if err = cache.WriteValue(writer, s.Miner); err != nil {
		return err
	}

	// ParentHash
	if err = cache.WriteValue(writer, &s.ParentHash); err != nil {
		return err
	}

	// Timestamp
	if err = cache.WriteValue(writer, s.Timestamp); err != nil {
		return err
	}

	// Transactions
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
	if err = cache.WriteValue(writer, txHashes); err != nil {
		return err
	}

	// Uncles
	if err = cache.WriteValue(writer, s.Uncles); err != nil {
		return err
	}

	// Withdrawals
	withdrawals := make([]cache.Marshaler, 0, len(s.Withdrawals))
	for _, withdrawal := range s.Withdrawals {
		withdrawals = append(withdrawals, &withdrawal)
	}
	if err = cache.WriteValue(writer, withdrawals); err != nil {
		return err
	}

	return nil
}

func (s *SimpleBlock[string]) UnmarshalCache(version uint64, reader io.Reader) (err error) {
	// BaseFeePerGas
	if err = cache.ReadValue(reader, &s.BaseFeePerGas, version); err != nil {
		return err
	}

	// BlockNumber
	if err = cache.ReadValue(reader, &s.BlockNumber, version); err != nil {
		return err
	}

	// Difficulty
	if err = cache.ReadValue(reader, &s.Difficulty, version); err != nil {
		return err
	}

	// GasLimit
	if err = cache.ReadValue(reader, &s.GasLimit, version); err != nil {
		return err
	}

	// GasUsed
	if err = cache.ReadValue(reader, &s.GasUsed, version); err != nil {
		return err
	}

	// Hash
	if err = cache.ReadValue(reader, &s.Hash, version); err != nil {
		return err
	}

	// Miner
	if err = cache.ReadValue(reader, &s.Miner, version); err != nil {
		return err
	}

	// ParentHash
	if err = cache.ReadValue(reader, &s.ParentHash, version); err != nil {
		return err
	}

	// Timestamp
	if err = cache.ReadValue(reader, &s.Timestamp, version); err != nil {
		return err
	}

	// Transactions
	s.Transactions = make([]string, 0)
	if err = cache.ReadValue(reader, &s.Transactions, version); err != nil {
		return err
	}

	// Uncles
	s.Uncles = make([]base.Hash, 0)
	if err = cache.ReadValue(reader, &s.Uncles, version); err != nil {
		return err
	}

	// Withdrawals
	s.Withdrawals = make([]SimpleWithdrawal, 0)
	if err = cache.ReadValue(reader, &s.Withdrawals, version); err != nil {
		return err
	}

	s.FinishUnmarshal()

	return nil
}

func (s *SimpleBlock[Tx]) FinishUnmarshal() {
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
//

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
	// TODO: This copy of an array possibly doesn't do what we expect
	target.Uncles = s.Uncles
	target.Withdrawals = s.Withdrawals
	target.raw = s.raw
}

// EXISTING_CODE

