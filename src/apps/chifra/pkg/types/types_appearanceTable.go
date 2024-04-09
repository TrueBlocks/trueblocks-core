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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

type SimpleAddrRecord struct {
	Address base.Address `json:"address"`
	Offset  uint32       `json:"offset"`
	Count   uint32       `json:"count"`
}

type SimpleAppRecord struct {
	BlockNumber      uint32 `json:"blockNumber"`
	TransactionIndex uint32 `json:"transactionIndex"`
}

// EXISTING_CODE

type RawAppearanceTable struct {
	AddressRecord string `json:"AddressRecord"`
	Appearances   string `json:"Appearances"`
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleAppearanceTable struct {
	AddressRecord SimpleAddrRecord    `json:"addressRecord"`
	Appearances   []SimpleAppRecord   `json:"appearances"`
	raw           *RawAppearanceTable `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimpleAppearanceTable) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *SimpleAppearanceTable) Raw() *RawAppearanceTable {
	return s.raw
}

func (s *SimpleAppearanceTable) SetRaw(raw *RawAppearanceTable) {
	s.raw = raw
}

func (s *SimpleAppearanceTable) Model(chain, format string, verbose bool, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]any{
		"address":     s.AddressRecord.Address.Hex(),
		"offset":      s.AddressRecord.Offset,
		"count":       s.AddressRecord.Count,
		"appearances": s.Appearances,
	}
	order = []string{
		"address",
		"offset",
		"count",
		"appearances",
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *SimpleAppearanceTable) FinishUnmarshal() {
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
