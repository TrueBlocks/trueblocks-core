// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

type simpleAppearanceTable struct {
	AddressRecord index.AddressRecord      `json:"addressRecord"`
	Appearances   []index.AppearanceRecord `json:"appearances"`
}

func (s *simpleAppearanceTable) Raw() *types.RawModeler {
	return nil
}

func (s *simpleAppearanceTable) Model(verbose bool, format string, extraOptions map[string]any) types.Model {
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
