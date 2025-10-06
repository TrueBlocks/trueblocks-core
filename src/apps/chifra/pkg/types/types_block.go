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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
)

// EXISTING_CODE

type Block struct {
	BaseFeePerGas base.Gas       `json:"baseFeePerGas"`
	BlockNumber   base.Blknum    `json:"blockNumber"`
	Difficulty    base.Value     `json:"difficulty"`
	GasLimit      base.Gas       `json:"gasLimit"`
	GasUsed       base.Gas       `json:"gasUsed"`
	Hash          base.Hash      `json:"hash"`
	Miner         base.Address   `json:"miner"`
	ParentHash    base.Hash      `json:"parentHash"`
	Timestamp     base.Timestamp `json:"timestamp"`
	Transactions  []Transaction  `json:"transactions"`
	Uncles        []base.Hash    `json:"uncles,omitempty"`
	Withdrawals   []Withdrawal   `json:"withdrawals,omitempty"`
	// EXISTING_CODE
	Number base.Blknum `json:"number"`
	// EXISTING_CODE
}

func (s Block) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *Block) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	props := &ModelProps{
		Chain:     chain,
		Format:    format,
		Verbose:   verbose,
		ExtraOpts: extraOpts,
	}

	rawNames := []Labeler{NewLabeler(s.Miner, "miner")}
	model := s.RawMap(props, rawNames)

	calcNames := []Labeler{}
	for k, v := range s.CalcMap(props, calcNames) {
		model[k] = v
	}

	var order = []string{}
	// EXISTING_CODE
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
		order = append(order, "transactions", "uncles")
		if len(s.Withdrawals) > 0 {
			order = append(order, "withdrawals")
		}
	} else {
		order = append(order, "transactionsCnt", "withdrawalsCnt")
	}

	for _, item := range append(rawNames, calcNames...) {
		key := item.name + "Name"
		if _, exists := model[key]; exists {
			order = append(order, key)
		}
	}
	order = reorderFields(order)
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// RawMap returns a map containing only the raw/base fields for this Block.
// This excludes any calculated or derived fields.
func (s *Block) RawMap(p *ModelProps, needed []Labeler) map[string]any {
	model := map[string]any{
		"gasUsed":       s.GasUsed,
		"gasLimit":      s.GasLimit,
		"hash":          s.Hash,
		"blockNumber":   s.BlockNumber,
		"parentHash":    s.ParentHash,
		"miner":         s.Miner.Hex(),
		"difficulty":    s.Difficulty,
		"timestamp":     s.Timestamp,
		"baseFeePerGas": s.BaseFeePerGas,
	}

	if p.Format == "json" {
		// If we wanted just transactions' hashes, we would return earlier. So here we know that we
		// have transactions as objects and want to load models for them to be able to display them
		txs, ok := any(s.Transactions).([]Transaction)
		if ok {
			items := make([]map[string]any, 0, len(txs))
			for _, txObject := range txs {
				items = append(items, txObject.Model(p.Chain, p.Format, p.Verbose, p.ExtraOpts).Data)
			}
			model["transactions"] = items
		} else {
			model["transactions"] = s.Transactions
		}
		if s.Uncles == nil {
			model["uncles"] = []base.Hash{}
		} else {
			model["uncles"] = s.Uncles
		}
		if len(s.Withdrawals) > 0 {
			withs := make([]map[string]any, 0, len(s.Withdrawals))
			for _, w := range s.Withdrawals {
				withs = append(withs, w.Model(p.Chain, p.Format, p.Verbose, p.ExtraOpts).Data)
			}
			model["withdrawals"] = withs
		} else {
			model["withdrawals"] = []Withdrawal{}
		}
	} else {
		model["transactionsCnt"] = len(s.Transactions)
		model["withdrawalsCnt"] = len(s.Withdrawals)
	}

	return labelAddresses(p, model, needed)
}

// CalcMap returns a map containing the calculated/derived fields for this Block.
// This includes formatted dates and other computed values.
func (s *Block) CalcMap(p *ModelProps, needed []Labeler) map[string]any {
	model := map[string]any{
		"date": s.Date(),
	}

	return labelAddresses(p, model, needed)
}

func (s *Block) Date() string {
	return base.FormattedDate(s.Timestamp)
}

func (s *Block) CacheLocations() (string, string, string) {
	paddedId := fmt.Sprintf("%09d", s.BlockNumber)
	parts := make([]string, 3)
	parts[0] = paddedId[:2]
	parts[1] = paddedId[2:4]
	parts[2] = paddedId[4:6]
	subFolder := strings.ToLower("Block") + "s"
	directory := filepath.Join(subFolder, filepath.Join(parts...))
	return directory, paddedId, "bin"
}

func (s *Block) MarshalCache(writer io.Writer) (err error) {
	// BaseFeePerGas
	if err = base.WriteValue(writer, s.BaseFeePerGas); err != nil {
		return err
	}

	// BlockNumber
	if err = base.WriteValue(writer, s.BlockNumber); err != nil {
		return err
	}

	// Difficulty
	if err = base.WriteValue(writer, s.Difficulty); err != nil {
		return err
	}

	// GasLimit
	if err = base.WriteValue(writer, s.GasLimit); err != nil {
		return err
	}

	// GasUsed
	if err = base.WriteValue(writer, s.GasUsed); err != nil {
		return err
	}

	// Hash
	if err = base.WriteValue(writer, &s.Hash); err != nil {
		return err
	}

	// Miner
	if err = base.WriteValue(writer, s.Miner); err != nil {
		return err
	}

	// ParentHash
	if err = base.WriteValue(writer, &s.ParentHash); err != nil {
		return err
	}

	// Timestamp
	if err = base.WriteValue(writer, s.Timestamp); err != nil {
		return err
	}

	// Transactions
	txHashes := make([]string, 0, len(s.Transactions))
	for _, tx := range s.Transactions {
		txHashes = append(txHashes, tx.Hash.Hex())
	}
	if err = base.WriteValue(writer, txHashes); err != nil {
		return err
	}

	// Uncles
	if err = base.WriteValue(writer, s.Uncles); err != nil {
		return err
	}

	// Withdrawals
	withdrawals := make([]base.Marshaler, 0, len(s.Withdrawals))
	for _, withdrawal := range s.Withdrawals {
		withdrawals = append(withdrawals, &withdrawal)
	}
	if err = base.WriteValue(writer, withdrawals); err != nil {
		return err
	}

	return nil
}

func (s *Block) UnmarshalCache(fileVersion uint64, reader io.Reader) (err error) {
	// Check for compatibility and return cache.ErrIncompatibleVersion to invalidate this item (see #3638)
	// EXISTING_CODE
	// EXISTING_CODE

	// BaseFeePerGas
	vBaseFeePerGas := version.NewVersion("2.5.8")
	if fileVersion <= vBaseFeePerGas.Uint64() {
		var val base.Wei
		if err = base.ReadValue(reader, &val, fileVersion); err != nil {
			return err
		}
		s.BaseFeePerGas = weiToGas(val)
	} else {
		// BaseFeePerGas
		if err = base.ReadValue(reader, &s.BaseFeePerGas, fileVersion); err != nil {
			return err
		}
	}

	// BlockNumber
	if err = base.ReadValue(reader, &s.BlockNumber, fileVersion); err != nil {
		return err
	}

	// Difficulty
	if err = base.ReadValue(reader, &s.Difficulty, fileVersion); err != nil {
		return err
	}

	// GasLimit
	if err = base.ReadValue(reader, &s.GasLimit, fileVersion); err != nil {
		return err
	}

	// GasUsed
	if err = base.ReadValue(reader, &s.GasUsed, fileVersion); err != nil {
		return err
	}

	// Hash
	if err = base.ReadValue(reader, &s.Hash, fileVersion); err != nil {
		return err
	}

	// Miner
	if err = base.ReadValue(reader, &s.Miner, fileVersion); err != nil {
		return err
	}

	// ParentHash
	if err = base.ReadValue(reader, &s.ParentHash, fileVersion); err != nil {
		return err
	}

	// Timestamp
	if err = base.ReadValue(reader, &s.Timestamp, fileVersion); err != nil {
		return err
	}

	// Transactions
	hashes := make([]string, 0, len(s.Transactions))
	if err = base.ReadValue(reader, &hashes, fileVersion); err != nil {
		return err
	}
	s.Transactions = make([]Transaction, 0, len(hashes))
	for i := 0; i < len(hashes); i++ {
		s.Transactions[i].Hash = base.HexToHash(hashes[i])
	}

	// Uncles
	s.Uncles = make([]base.Hash, 0)
	if err = base.ReadValue(reader, &s.Uncles, fileVersion); err != nil {
		return err
	}

	// Withdrawals
	s.Withdrawals = make([]Withdrawal, 0)
	if err = base.ReadValue(reader, &s.Withdrawals, fileVersion); err != nil {
		return err
	}

	s.FinishUnmarshal(fileVersion)

	return nil
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *Block) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
//

func weiToGas(w base.Wei) base.Gas {
	return base.Gas(w.Uint64())
}

// EXISTING_CODE
