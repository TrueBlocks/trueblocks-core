// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package chunksPkg

// EXISTING_CODE
import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// EXISTING_CODE

type simpleChunkStats struct {
	AddrsPerBlock float64 `json:"addrsPerBlock"`
	AppsPerAddr   float64 `json:"appsPerAddr"`
	AppsPerBlock  float64 `json:"appsPerBlock"`
	BloomSz       uint64  `json:"bloomSz"`
	ChunkSz       uint64  `json:"chunkSz"`
	NAddrs        uint64  `json:"nAddrs"`
	NApps         uint64  `json:"nApps"`
	NBlocks       uint64  `json:"nBlocks"`
	NBlooms       uint64  `json:"nBlooms"`
	Range         string  `json:"range"`
	RangeEnd      string  `json:"rangeEnd"`
	Ratio         float64 `json:"ratio"`
	RecWid        uint64  `json:"recWid"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *simpleChunkStats) Raw() *types.RawModeler {
	return nil
}

func (s *simpleChunkStats) Model(verbose bool, format string, extraOptions map[string]any) types.Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]any{
		"addrsPerBlock": s.AddrsPerBlock,
		"appsPerAddr":   s.AppsPerAddr,
		"appsPerBlock":  s.AppsPerBlock,
		"bloomSz":       s.BloomSz,
		"chunkSz":       s.ChunkSz,
		"range":         s.Range,
		"nAddrs":        s.NAddrs,
		"nApps":         s.NApps,
		"nBlocks":       s.NBlocks,
		"nBlooms":       s.NBlooms,
		"ratio":         s.Ratio,
		"recWid":        s.RecWid,
		"rangeEnd":      s.RangeEnd,
	}
	order = []string{
		"range",
		"rangeEnd",
		"nAddrs",
		"nApps",
		"nBlocks",
		"nBlooms",
		"recWid",
		"bloomSz",
		"chunkSz",
		"addrsPerBlock",
		"appsPerBlock",
		"appsPerAddr",
		"ratio",
	}
	// EXISTING_CODE

	return types.Model{
		Data:  model,
		Order: order,
	}
}

// EXISTING_CODE
// EXISTING_CODE
