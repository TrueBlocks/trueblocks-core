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

func (s *Withdrawal) Model(chain, format string, verbose bool, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]interface{}{
		"address":        s.Address,
		"amount":         s.Amount.String(),
		"blockNumber":    s.BlockNumber,
		"date":           s.Date(),
		"index":          s.Index,
		"timestamp":      s.Timestamp,
		"validatorIndex": s.ValidatorIndex,
	}

	order = []string{
		"blockNumber",
		"index",
		"validatorIndex",
		"address",
		"timestamp",
		"date",
		"amount",
	}

	asEther := extraOptions["ether"] == true
	if asEther {
		model["ether"] = s.Amount.ToEtherStr(18)
		order = append(order, "ether")
	}

	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *Withdrawal) Date() string {
	return base.FormattedDate(s.Timestamp)
}

type WithdrawalGroup struct {
	BlockNumber      base.Blknum
	TransactionIndex base.Txnum
	Withdrawals      []Withdrawal
}

func (s *WithdrawalGroup) CacheName() string {
	return "Withdrawal"
}

func (s *WithdrawalGroup) CacheId() string {
	return fmt.Sprintf("%09d", s.BlockNumber)
}

func (s *WithdrawalGroup) CacheLocation() (directory string, extension string) {
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

func (s *WithdrawalGroup) MarshalCache(writer io.Writer) (err error) {
	return cache.WriteValue(writer, s.Withdrawals)
}

func (s *WithdrawalGroup) UnmarshalCache(vers uint64, reader io.Reader) (err error) {
	return cache.ReadValue(reader, &s.Withdrawals, vers)
}

func (s *Withdrawal) MarshalCache(writer io.Writer) (err error) {
	// Address
	if err = cache.WriteValue(writer, s.Address); err != nil {
		return err
	}

	// Amount
	if err = cache.WriteValue(writer, &s.Amount); err != nil {
		return err
	}

	// BlockNumber
	if err = cache.WriteValue(writer, s.BlockNumber); err != nil {
		return err
	}

	// Index
	if err = cache.WriteValue(writer, s.Index); err != nil {
		return err
	}

	// Timestamp
	if err = cache.WriteValue(writer, s.Timestamp); err != nil {
		return err
	}

	// ValidatorIndex
	if err = cache.WriteValue(writer, s.ValidatorIndex); err != nil {
		return err
	}

	return nil
}

func (s *Withdrawal) UnmarshalCache(vers uint64, reader io.Reader) (err error) {
	// Check for compatibility and return cache.ErrIncompatibleVersion to invalidate this item (see #3638)
	// EXISTING_CODE
	// EXISTING_CODE

	// Address
	if err = cache.ReadValue(reader, &s.Address, vers); err != nil {
		return err
	}

	// Amount
	if err = cache.ReadValue(reader, &s.Amount, vers); err != nil {
		return err
	}

	// BlockNumber
	if err = cache.ReadValue(reader, &s.BlockNumber, vers); err != nil {
		return err
	}

	// Index
	if err = cache.ReadValue(reader, &s.Index, vers); err != nil {
		return err
	}

	// Timestamp
	if err = cache.ReadValue(reader, &s.Timestamp, vers); err != nil {
		return err
	}

	// ValidatorIndex
	if err = cache.ReadValue(reader, &s.ValidatorIndex, vers); err != nil {
		return err
	}

	s.FinishUnmarshal()

	return nil
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *Withdrawal) FinishUnmarshal() {
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
