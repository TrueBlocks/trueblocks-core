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

type Approval struct {
	Address     base.Address   `json:"address"`
	AddressName string         `json:"addressName"`
	Amount      base.Wei       `json:"amount"`
	BlockNumber base.Blknum    `json:"blockNumber"`
	Spender     base.Address   `json:"spender"`
	SpenderName string         `json:"spenderName"`
	Timestamp   base.Timestamp `json:"timestamp"`
	Token       base.Address   `json:"token"`
	TokenName   string         `json:"tokenName"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s Approval) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *Approval) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	_ = chain
	_ = format
	_ = verbose
	_ = extraOpts
	var model = map[string]any{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]any{
		"blockNumber": s.BlockNumber,
		"timestamp":   s.Timestamp,
		"date":        s.Date(),
		"owner":       s.Address,
		"spender":     s.Spender,
		"token":       s.Token,
		"amount":      s.Amount.String(),
	}

	order = []string{
		"blockNumber",
		"timestamp",
		"date",
		"owner",
		"spender",
		"token",
		"amount",
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *Approval) Date() string {
	return base.FormattedDate(s.Timestamp)
}

func (s *Approval) CacheLocations() (string, string, string) {
	paddedId := fmt.Sprintf("%s-%09d", s.Address.Hex()[2:], s.BlockNumber)
	parts := make([]string, 3)
	parts[0] = paddedId[:2]
	parts[1] = paddedId[2:4]
	parts[2] = paddedId[4:6]
	subFolder := strings.ToLower("Approval") + "s"
	directory := filepath.Join(subFolder, filepath.Join(parts...))
	return directory, paddedId, "bin"
}

func (s *Approval) MarshalCache(writer io.Writer) (err error) {
	// Address
	if err = base.WriteValue(writer, s.Address); err != nil {
		return err
	}

	// AddressName
	if err = base.WriteValue(writer, s.AddressName); err != nil {
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

	// Spender
	if err = base.WriteValue(writer, s.Spender); err != nil {
		return err
	}

	// SpenderName
	if err = base.WriteValue(writer, s.SpenderName); err != nil {
		return err
	}

	// Timestamp
	if err = base.WriteValue(writer, s.Timestamp); err != nil {
		return err
	}

	// Token
	if err = base.WriteValue(writer, s.Token); err != nil {
		return err
	}

	// TokenName
	if err = base.WriteValue(writer, s.TokenName); err != nil {
		return err
	}

	return nil
}

func (s *Approval) UnmarshalCache(fileVersion uint64, reader io.Reader) (err error) {
	// Check for compatibility and return cache.ErrIncompatibleVersion to invalidate this item (see #3638)
	// EXISTING_CODE
	// EXISTING_CODE

	// Address
	if err = base.ReadValue(reader, &s.Address, fileVersion); err != nil {
		return err
	}

	// AddressName
	if err = base.ReadValue(reader, &s.AddressName, fileVersion); err != nil {
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

	// Spender
	if err = base.ReadValue(reader, &s.Spender, fileVersion); err != nil {
		return err
	}

	// SpenderName
	if err = base.ReadValue(reader, &s.SpenderName, fileVersion); err != nil {
		return err
	}

	// Timestamp
	if err = base.ReadValue(reader, &s.Timestamp, fileVersion); err != nil {
		return err
	}

	// Token
	if err = base.ReadValue(reader, &s.Token, fileVersion); err != nil {
		return err
	}

	// TokenName
	if err = base.ReadValue(reader, &s.TokenName, fileVersion); err != nil {
		return err
	}

	s.FinishUnmarshal(fileVersion)

	return nil
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *Approval) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
