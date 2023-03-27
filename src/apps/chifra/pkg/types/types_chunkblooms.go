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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

// EXISTING_CODE

type RawChunkBlooms struct {
	Count     string `json:"count"`
	Hash      string `json:"hash"`
	Magic     string `json:"magic"`
	NInserted string `json:"nInserted"`
	Range     string `json:"range"`
	Size      string `json:"size"`
	Width     string `json:"width"`
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleChunkBlooms struct {
	Count     uint64          `json:"count"`
	Hash      base.Hash       `json:"hash"`
	Magic     string          `json:"magic"`
	NInserted uint64          `json:"nInserted"`
	Range     base.FileRange  `json:"range"`
	Size      uint64          `json:"size"`
	Width     uint64          `json:"width"`
	raw       *RawChunkBlooms `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimpleChunkBlooms) Raw() *RawChunkBlooms {
	return s.raw
}

func (s *SimpleChunkBlooms) SetRaw(raw *RawChunkBlooms) {
	s.raw = raw
}

func (s *SimpleChunkBlooms) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]any{
		"count":     s.Count,
		"hash":      s.Hash,
		"magic":     "0x" + s.Magic,
		"nInserted": s.NInserted,
		"range":     s.Range,
		"size":      s.Size,
		"width":     s.Width,
	}
	order = []string{
		"range",
		"magic",
		"hash",
		"count",
		"nInserted",
		"size",
		"width",
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *SimpleChunkBlooms) WriteTo(w io.Writer) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

func (s *SimpleChunkBlooms) ReadFrom(r io.Reader) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

// EXISTING_CODE
// EXISTING_CODE
