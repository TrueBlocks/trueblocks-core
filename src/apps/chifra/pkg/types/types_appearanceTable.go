// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package types

// EXISTING_CODE
import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

type AddrRecord struct {
	Address base.Address `json:"address"`
	Offset  uint32       `json:"offset"`
	Count   uint32       `json:"count"`
}

type AppRecord struct {
	BlockNumber      uint32 `json:"blockNumber"`
	TransactionIndex uint32 `json:"transactionIndex"`
}

// EXISTING_CODE

type RawAppearanceTable struct {
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleAppearanceTable struct {
	AddressRecord AddrRecord  `json:"addressRecord"`
	Appearances   []AppRecord `json:"appearances"`
	raw           *RawAppearanceTable
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimpleAppearanceTable) Raw() *RawAppearanceTable {
	return nil
}

func (s *SimpleAppearanceTable) SetRaw(r *RawAppearanceTable) {
	s.raw = r
}

func (s *SimpleAppearanceTable) Model(chain, format string, verbose bool, extraOptions map[string]any) Model {
	// EXISTING_CODE
	return Model{
		Data: map[string]any{
			"address":     s.AddressRecord.Address.Hex(),
			"offset":      s.AddressRecord.Offset,
			"count":       s.AddressRecord.Count,
			"appearances": s.Appearances,
		},
		Order: []string{
			"address",
			"offset",
			"count",
			"appearances",
		},
	}
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE

// EXISTING_CODE
// EXISTING_CODE
