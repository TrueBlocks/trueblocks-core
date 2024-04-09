// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// AddrRecord is a single record in the Address table
type AddrRecord struct {
	Address base.Address `json:"address"`
	Offset  uint32       `json:"offset"`
	Count   uint32       `json:"count"`
}

type AppRecord struct {
	BlockNumber      uint32 `json:"blockNumber"`
	TransactionIndex uint32 `json:"transactionIndex"`
}

type simpleAppearanceTable struct {
	AddressRecord AddrRecord  `json:"addressRecord"`
	Appearances   []AppRecord `json:"appearances"`
}

func (s *simpleAppearanceTable) Raw() *types.RawModeler {
	return nil
}

func (s *simpleAppearanceTable) Model(chain, format string, verbose bool, extraOptions map[string]any) types.Model {
	return types.Model{
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
}
