// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package types

// EXISTING_CODE
import "encoding/json"

// EXISTING_CODE

type ChunkStats struct {
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

func (s ChunkStats) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *ChunkStats) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	var model = map[string]any{}
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

	return Model{
		Data:  model,
		Order: order,
	}
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *ChunkStats) FinishUnmarshal() {
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
