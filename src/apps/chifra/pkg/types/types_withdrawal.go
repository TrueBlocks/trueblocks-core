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

type RawWithdrawal struct {
	Address        string `json:"address"`
	Amount         string `json:"amount"`
	BlockNumber    string `json:"blockNumber"`
	Index          string `json:"index"`
	Timestamp      string `json:"timestamp"`
	ValidatorIndex string `json:"validatorIndex"`
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleWithdrawal struct {
	Address        base.Address   `json:"address"`
	Amount         base.Wei       `json:"amount"`
	BlockNumber    base.Blknum    `json:"blockNumber"`
	Index          uint64         `json:"index"`
	Timestamp      base.Timestamp `json:"timestamp"`
	ValidatorIndex uint64         `json:"validatorIndex"`
	raw            *RawWithdrawal `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimpleWithdrawal) Raw() *RawWithdrawal {
	return s.raw
}

func (s *SimpleWithdrawal) SetRaw(raw *RawWithdrawal) {
	s.raw = raw
}

func (s *SimpleWithdrawal) Model(chain, format string, verbose bool, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	asEther := extraOptions["ether"] == true
	model = map[string]interface{}{
		"address":        s.Address,
		"amount":         base.FormattedValue(s.Amount, asEther, 18),
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
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *SimpleWithdrawal) Date() string {
	return utils.FormattedDate(s.Timestamp)
}

// --> cacheable by block as group
type SimpleWithdrawalGroup struct {
	BlockNumber      base.Blknum
	TransactionIndex base.Txnum
	Withdrawals      []SimpleWithdrawal
}

func (s *SimpleWithdrawalGroup) CacheName() string {
	return "Withdrawal"
}

func (s *SimpleWithdrawalGroup) CacheId() string {
	return fmt.Sprintf("%09d", s.BlockNumber)
}

func (s *SimpleWithdrawalGroup) CacheLocation() (directory string, extension string) {
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

func (s *SimpleWithdrawalGroup) MarshalCache(writer io.Writer) (err error) {
	return cache.WriteValue(writer, s.Withdrawals)
}

func (s *SimpleWithdrawalGroup) UnmarshalCache(version uint64, reader io.Reader) (err error) {
	return cache.ReadValue(reader, &s.Withdrawals, version)
}

func (s *SimpleWithdrawal) MarshalCache(writer io.Writer) (err error) {
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

func (s *SimpleWithdrawal) UnmarshalCache(version uint64, reader io.Reader) (err error) {
	// Address
	if err = cache.ReadValue(reader, &s.Address, version); err != nil {
		return err
	}

	// Amount
	if err = cache.ReadValue(reader, &s.Amount, version); err != nil {
		return err
	}

	// BlockNumber
	if err = cache.ReadValue(reader, &s.BlockNumber, version); err != nil {
		return err
	}

	// Index
	if err = cache.ReadValue(reader, &s.Index, version); err != nil {
		return err
	}

	// Timestamp
	if err = cache.ReadValue(reader, &s.Timestamp, version); err != nil {
		return err
	}

	// ValidatorIndex
	if err = cache.ReadValue(reader, &s.ValidatorIndex, version); err != nil {
		return err
	}

	s.FinishUnmarshal()

	return nil
}

func (s *SimpleWithdrawal) FinishUnmarshal() {
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE

