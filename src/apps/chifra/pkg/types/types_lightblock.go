// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package types

// EXISTING_CODE
import (
	"encoding/json"
	"fmt"
	"io"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
)

// EXISTING_CODE

type RawLightBlock struct {
	Author           string       `json:"author"`
	BaseFeePerGas    string       `json:"baseFeePerGas"`
	BlockNumber      string       `json:"number"`
	Difficulty       string       `json:"difficulty"`
	ExtraData        string       `json:"extraData"`
	GasLimit         string       `json:"gasLimit"`
	GasUsed          string       `json:"gasUsed"`
	Hash             string       `json:"hash"`
	LogsBloom        string       `json:"logsBloom"`
	Miner            string       `json:"miner"`
	MixHash          string       `json:"mixHash"`
	Nonce            string       `json:"nonce"`
	ParentHash       string       `json:"parentHash"`
	ReceiptsRoot     string       `json:"receiptsRoot"`
	Sha3Uncles       string       `json:"sha3Uncles"`
	Size             string       `json:"size"`
	StateRoot        string       `json:"stateRoot"`
	Timestamp        string       `json:"timestamp"`
	TotalDifficulty  string       `json:"totalDifficulty"`
	Transactions     []string     `json:"transactions"`
	TransactionsRoot string       `json:"transactionsRoot"`
	Uncles           []string     `json:"uncles"`
	Withdrawals      []Withdrawal `json:"withdrawals"`
}

type LightBlock struct {
	BaseFeePerGas base.Gas       `json:"baseFeePerGas"`
	BlockNumber   base.Blknum    `json:"blockNumber"`
	Difficulty    base.Value     `json:"difficulty"`
	GasLimit      base.Gas       `json:"gasLimit"`
	GasUsed       base.Gas       `json:"gasUsed"`
	Hash          base.Hash      `json:"hash"`
	Miner         base.Address   `json:"miner"`
	ParentHash    base.Hash      `json:"parentHash"`
	Timestamp     base.Timestamp `json:"timestamp"`
	Transactions  []string       `json:"transactions"`
	Uncles        []base.Hash    `json:"uncles,omitempty"`
	Withdrawals   []Withdrawal   `json:"withdrawals,omitempty"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s LightBlock) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *LightBlock) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]interface{}{
		"hash":        s.Hash,
		"blockNumber": s.BlockNumber,
		"parentHash":  s.ParentHash,
		"timestamp":   s.Timestamp,
		"date":        s.Date(),
	}
	order = []string{
		"hash",
		"blockNumber",
		"parentHash",
		"timestamp",
		"date",
	}

	if format == "json" {
		model["tx_hashes"] = s.Transactions
		if s.BlockNumber >= base.KnownBlock(chain, base.Shanghai) {
			withs := make([]map[string]any, 0, len(s.Withdrawals))
			for _, w := range s.Withdrawals {
				withs = append(withs, w.Model(chain, format, verbose, extraOpts).Data)
			}
			model["withdrawals"] = withs
		}
	} else {
		model["transactionsCnt"] = len(s.Transactions)
		order = append(order, "transactionsCnt")
		if s.BlockNumber > base.KnownBlock(chain, base.Shanghai) {
			model["withdrawalsCnt"] = len(s.Withdrawals)
			order = append(order, "withdrawalsCnt")
		}
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *LightBlock) Date() string {
	return base.FormattedDate(s.Timestamp)
}

func (s *LightBlock) CacheName() string {
	return "Block"
}

func (s *LightBlock) CacheId() string {
	return fmt.Sprintf("%09d", s.BlockNumber)
}

func (s *LightBlock) CacheLocation() (directory string, extension string) {
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

func (s *LightBlock) MarshalCache(writer io.Writer) (err error) {
	// BaseFeePerGas
	if err = cache.WriteValue(writer, s.BaseFeePerGas); err != nil {
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
	if err = cache.WriteValue(writer, s.Transactions); err != nil {
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

func (s *LightBlock) UnmarshalCache(vers uint64, reader io.Reader) (err error) {
	// Check for compatibility and return cache.ErrIncompatibleVersion to invalidate this item (see #3638)
	// EXISTING_CODE
	// EXISTING_CODE

	// BaseFeePerGas
	vBaseFeePerGas := version.NewVersion("2.5.8")
	if vers <= vBaseFeePerGas.Uint64() {
		var val base.Wei
		if err = cache.ReadValue(reader, &val, vers); err != nil {
			return err
		}
		s.BaseFeePerGas = weiToGas(val)
	} else {
		// BaseFeePerGas
		if err = cache.ReadValue(reader, &s.BaseFeePerGas, vers); err != nil {
			return err
		}
	}

	// BlockNumber
	if err = cache.ReadValue(reader, &s.BlockNumber, vers); err != nil {
		return err
	}

	// Difficulty
	if err = cache.ReadValue(reader, &s.Difficulty, vers); err != nil {
		return err
	}

	// GasLimit
	if err = cache.ReadValue(reader, &s.GasLimit, vers); err != nil {
		return err
	}

	// GasUsed
	if err = cache.ReadValue(reader, &s.GasUsed, vers); err != nil {
		return err
	}

	// Hash
	if err = cache.ReadValue(reader, &s.Hash, vers); err != nil {
		return err
	}

	// Miner
	if err = cache.ReadValue(reader, &s.Miner, vers); err != nil {
		return err
	}

	// ParentHash
	if err = cache.ReadValue(reader, &s.ParentHash, vers); err != nil {
		return err
	}

	// Timestamp
	if err = cache.ReadValue(reader, &s.Timestamp, vers); err != nil {
		return err
	}

	// Transactions
	s.Transactions = make([]string, 0)
	if err = cache.ReadValue(reader, &s.Transactions, vers); err != nil {
		return err
	}

	// Uncles
	s.Uncles = make([]base.Hash, 0)
	if err = cache.ReadValue(reader, &s.Uncles, vers); err != nil {
		return err
	}

	// Withdrawals
	s.Withdrawals = make([]Withdrawal, 0)
	if err = cache.ReadValue(reader, &s.Withdrawals, vers); err != nil {
		return err
	}

	s.FinishUnmarshal()

	return nil
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *LightBlock) FinishUnmarshal() {
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
