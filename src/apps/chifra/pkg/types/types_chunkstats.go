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
	"io"
)

// EXISTING_CODE

type RawChunkStats struct {
	AddrsPerBlock string `json:"addrsPerBlock"`
	AppsPerAddr   string `json:"appsPerAddr"`
	AppsPerBlock  string `json:"appsPerBlock"`
	BloomSz       string `json:"bloomSz"`
	ChunkSz       string `json:"chunkSz"`
	End           string `json:"end"`
	NAddrs        string `json:"nAddrs"`
	NApps         string `json:"nApps"`
	NBlocks       string `json:"nBlocks"`
	NBlooms       string `json:"nBlooms"`
	Ratio         string `json:"ratio"`
	RecWid        string `json:"recWid"`
	Start         string `json:"start"`
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleChunkStats struct {
	AddrsPerBlock float64        `json:"addrsPerBlock"`
	AppsPerAddr   float64        `json:"appsPerAddr"`
	AppsPerBlock  float64        `json:"appsPerBlock"`
	BloomSz       uint64         `json:"bloomSz"`
	ChunkSz       uint64         `json:"chunkSz"`
	End           uint64         `json:"end"`
	NAddrs        uint64         `json:"nAddrs"`
	NApps         uint64         `json:"nApps"`
	NBlocks       uint64         `json:"nBlocks"`
	NBlooms       uint64         `json:"nBlooms"`
	Ratio         float64        `json:"ratio"`
	RecWid        uint64         `json:"recWid"`
	Start         uint64         `json:"start"`
	raw           *RawChunkStats `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimpleChunkStats) Raw() *RawChunkStats {
	return s.raw
}

func (s *SimpleChunkStats) SetRaw(raw *RawChunkStats) {
	s.raw = raw
}

func (s *SimpleChunkStats) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]any{
		"addrsPerBlock": s.AddrsPerBlock,
		"appsPerAddr":   s.AppsPerAddr,
		"appsPerBlock":  s.AppsPerBlock,
		"bloomSz":       s.BloomSz,
		"chunkSz":       s.ChunkSz,
		"end":           s.End,
		"nAddrs":        s.NAddrs,
		"nApps":         s.NApps,
		"nBlocks":       s.NBlocks,
		"nBlooms":       s.NBlooms,
		"ratio":         s.Ratio,
		"recWid":        s.RecWid,
		"start":         s.Start,
	}
	order = []string{
		"start",
		"end",
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

func (s *SimpleChunkStats) WriteTo(w io.Writer) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

func (s *SimpleChunkStats) ReadFrom(r io.Reader) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

// EXISTING_CODE
// EXISTING_CODE
