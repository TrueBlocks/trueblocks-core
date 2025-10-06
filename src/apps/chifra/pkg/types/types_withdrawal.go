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
)

// EXISTING_CODE

type Withdrawal struct {
	Address        base.Address   `json:"address"`
	Amount         base.Wei       `json:"amount"`
	BlockNumber    base.Blknum    `json:"blockNumber"`
	Index          base.Value     `json:"index"`
	Timestamp      base.Timestamp `json:"timestamp"`
	ValidatorIndex base.Value     `json:"validatorIndex"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s Withdrawal) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *Withdrawal) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	props := NewModelProps(chain, format, verbose, extraOpts)

	rawNames := []Labeler{
		NewLabeler(s.Address, "address"),
	}
	model := s.RawMap(props, &rawNames)
	for k, v := range s.CalcMap(props) {
		model[k] = v
	}

	var order = []string{}
	// EXISTING_CODE
	order = []string{
		"blockNumber",
		"index",
		"validatorIndex",
		"address",
		"timestamp",
		"date",
		"amount",
	}

	if extraOpts["ether"] == true {
		order = append(order, "ether")
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

// RawMap returns a map containing only the raw/base fields for this Withdrawal.
func (s *Withdrawal) RawMap(p *ModelProps, needed *[]Labeler) map[string]any {
	model := map[string]any{
		// EXISTING_CODE
		"address":        s.Address,
		"amount":         s.Amount.String(),
		"blockNumber":    s.BlockNumber,
		"index":          s.Index,
		"timestamp":      s.Timestamp,
		"validatorIndex": s.ValidatorIndex,
		// EXISTING_CODE
	}

	// EXISTING_CODE
	// EXISTING_CODE

	return labelAddresses(p, model, needed)
}

// CalcMap returns a map containing the calculated/derived fields for this Withdrawal.
func (s *Withdrawal) CalcMap(p *ModelProps) map[string]any {
	model := map[string]any{
		// EXISTING_CODE
		"date": s.Date(),
		// EXISTING_CODE
	}

	// EXISTING_CODE
	if p.ExtraOpts["ether"] == true {
		model["ether"] = s.Amount.ToFloatString(18)
	}
	// EXISTING_CODE

	return model
}

func (s *Withdrawal) Date() string {
	return base.FormattedDate(s.Timestamp)
}

func (s *WithdrawalGroup) CacheLocations() (string, string, string) {
	paddedId := fmt.Sprintf("%09d", s.BlockNumber)
	parts := make([]string, 3)
	parts[0] = paddedId[:2]
	parts[1] = paddedId[2:4]
	parts[2] = paddedId[4:6]
	subFolder := strings.ToLower("Withdrawal") + "s"
	directory := filepath.Join(subFolder, filepath.Join(parts...))
	return directory, paddedId, "bin"
}

type WithdrawalGroup struct {
	BlockNumber      base.Blknum
	TransactionIndex base.Txnum
	Withdrawals      []Withdrawal
}

func (s *WithdrawalGroup) MarshalCache(writer io.Writer) (err error) {
	return base.WriteValue(writer, s.Withdrawals)
}

func (s *WithdrawalGroup) UnmarshalCache(fileVersion uint64, reader io.Reader) (err error) {
	return base.ReadValue(reader, &s.Withdrawals, fileVersion)
}

func (s *Withdrawal) MarshalCache(writer io.Writer) (err error) {
	// Address
	if err = base.WriteValue(writer, s.Address); err != nil {
		return err
	}

	// Amount
	if err = base.WriteValue(writer, &s.Amount); err != nil {
		return err
	}

	// BlockNumber
	if err = base.WriteValue(writer, s.BlockNumber); err != nil {
		return err
	}

	// Index
	if err = base.WriteValue(writer, s.Index); err != nil {
		return err
	}

	// Timestamp
	if err = base.WriteValue(writer, s.Timestamp); err != nil {
		return err
	}

	// ValidatorIndex
	if err = base.WriteValue(writer, s.ValidatorIndex); err != nil {
		return err
	}

	return nil
}

func (s *Withdrawal) UnmarshalCache(fileVersion uint64, reader io.Reader) (err error) {
	// Check for compatibility and return cache.ErrIncompatibleVersion to invalidate this item (see #3638)
	// EXISTING_CODE
	// EXISTING_CODE

	// Address
	if err = base.ReadValue(reader, &s.Address, fileVersion); err != nil {
		return err
	}

	// Amount
	if err = base.ReadValue(reader, &s.Amount, fileVersion); err != nil {
		return err
	}

	// BlockNumber
	if err = base.ReadValue(reader, &s.BlockNumber, fileVersion); err != nil {
		return err
	}

	// Index
	if err = base.ReadValue(reader, &s.Index, fileVersion); err != nil {
		return err
	}

	// Timestamp
	if err = base.ReadValue(reader, &s.Timestamp, fileVersion); err != nil {
		return err
	}

	// ValidatorIndex
	if err = base.ReadValue(reader, &s.ValidatorIndex, fileVersion); err != nil {
		return err
	}

	s.FinishUnmarshal(fileVersion)

	return nil
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *Withdrawal) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
