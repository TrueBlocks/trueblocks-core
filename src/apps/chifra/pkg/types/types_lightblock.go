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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/version"
)

// EXISTING_CODE

type LightBlock struct {
	BaseFeePerGas base.Gas         `json:"baseFeePerGas"`
	BlockNumber   base.Blknum      `json:"blockNumber"`
	Difficulty    base.Value       `json:"difficulty"`
	GasLimit      base.Gas         `json:"gasLimit"`
	GasUsed       base.Gas         `json:"gasUsed"`
	Hash          base.Hash        `json:"hash"`
	Miner         base.Address     `json:"miner"`
	ParentHash    base.Hash        `json:"parentHash"`
	Timestamp     base.Timestamp   `json:"timestamp"`
	Transactions  []string         `json:"transactions"`
	Uncles        []base.Hash      `json:"uncles,omitempty"`
	Withdrawals   []Withdrawal     `json:"withdrawals,omitempty"`
	Calcs         *LightBlockCalcs `json:"calcs,omitempty"`
	// EXISTING_CODE
	Number base.Blknum `json:"number"`
	// EXISTING_CODE
}

func (s LightBlock) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *LightBlock) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	props := NewModelProps(chain, format, verbose, extraOpts)

	rawNames := []Labeler{
		NewLabeler(s.Miner, "miner"),
	}
	model := s.RawMap(props, &rawNames)
	for k, v := range s.CalcMap(props) {
		model[k] = v
	}

	var order = []string{}
	// EXISTING_CODE
	order = []string{
		"hash",
		"blockNumber",
		"parentHash",
		"timestamp",
		"date",
	}

	if format == "json" {
		order = append(order, "tx_hashes")
		if s.BlockNumber >= base.KnownBlock(chain, base.Shanghai) {
			order = append(order, "withdrawals")
		}
	} else {
		order = append(order, "transactionsCnt")
		if s.BlockNumber > base.KnownBlock(chain, base.Shanghai) {
			order = append(order, "withdrawalsCnt")
		}
	}
	// EXISTING_CODE

	for _, item := range rawNames {
		key := item.name + "Name"
		if _, exists := model[key]; exists {
			order = append(order, key)
		}
	}
	order = reorderFields(order)

	return Model{
		Data:  model,
		Order: order,
	}
}

// RawMap returns a map containing only the raw/base fields for this LightBlock.
func (s *LightBlock) RawMap(p *ModelProps, needed *[]Labeler) map[string]any {
	model := map[string]any{
		// EXISTING_CODE
		"hash":        s.Hash,
		"blockNumber": s.BlockNumber,
		"parentHash":  s.ParentHash,
		"timestamp":   s.Timestamp,
		"miner":       s.Miner,
		// EXISTING_CODE
	}

	// EXISTING_CODE
	if p.Format == "json" {
		model["tx_hashes"] = s.Transactions
		if s.BlockNumber >= base.KnownBlock(p.Chain, base.Shanghai) {
			withs := make([]map[string]any, 0, len(s.Withdrawals))
			for _, w := range s.Withdrawals {
				withs = append(withs, w.Model(p.Chain, p.Format, p.Verbose, p.ExtraOpts).Data)
			}
			model["withdrawals"] = withs
		}
	} else {
		model["transactionsCnt"] = len(s.Transactions)
		if s.BlockNumber > base.KnownBlock(p.Chain, base.Shanghai) {
			model["withdrawalsCnt"] = len(s.Withdrawals)
		}
	}
	// EXISTING_CODE

	return labelAddresses(p, model, needed)
}

// CalcMap returns a map containing the calculated/derived fields for this type.
func (s *LightBlock) CalcMap(p *ModelProps) map[string]any {
	_ = p // delint
	model := map[string]any{
		// EXISTING_CODE
		"date": s.Date(),
		// EXISTING_CODE
	}

	// EXISTING_CODE
	// EXISTING_CODE

	return model
}

func (s *LightBlock) Date() string {
	return base.FormattedDate(s.Timestamp)
}

func (s *LightBlock) CacheLocations() (string, string, string) {
	paddedId := fmt.Sprintf("%09d", s.BlockNumber)
	parts := make([]string, 3)
	parts[0] = paddedId[:2]
	parts[1] = paddedId[2:4]
	parts[2] = paddedId[4:6]
	subFolder := strings.ToLower("Block") + "s"
	directory := filepath.Join(subFolder, filepath.Join(parts...))
	return directory, paddedId, "bin"
}

func (s *LightBlock) MarshalCache(writer io.Writer) (err error) {
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
	if err = base.WriteValue(writer, s.Transactions); err != nil {
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

func (s *LightBlock) UnmarshalCache(fileVersion uint64, reader io.Reader) (err error) {
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
	s.Transactions = make([]string, 0)
	if err = base.ReadValue(reader, &s.Transactions, fileVersion); err != nil {
		return err
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
func (s *LightBlock) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	s.Calcs = nil
	// EXISTING_CODE
	// EXISTING_CODE
}

// LightBlockCalcs holds lazy-loaded calculated fields for LightBlock
type LightBlockCalcs struct {
	// EXISTING_CODE
	Date string `json:"date"`
	// EXISTING_CODE
}

func (s *LightBlock) EnsureCalcs(p *ModelProps, fieldFilter []string) error {
	_ = fieldFilter // delint
	if s.Calcs != nil {
		return nil
	}

	calcMap := s.CalcMap(p)
	if len(calcMap) == 0 {
		return nil
	}

	jsonBytes, err := json.Marshal(calcMap)
	if err != nil {
		return err
	}

	s.Calcs = &LightBlockCalcs{}
	return json.Unmarshal(jsonBytes, s.Calcs)
}

// EXISTING_CODE
// EXISTING_CODE
