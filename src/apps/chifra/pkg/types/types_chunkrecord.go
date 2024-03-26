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

// EXISTING_CODE

type RawChunkRecord struct {
	BloomHash string `json:"bloomHash"`
	BloomSize string `json:"bloomSize"`
	IndexHash string `json:"indexHash"`
	IndexSize string `json:"indexSize"`
	Range     string `json:"range"`
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleChunkRecord struct {
	BloomHash base.IpfsHash   `json:"bloomHash"`
	BloomSize int64           `json:"bloomSize"`
	IndexHash base.IpfsHash   `json:"indexHash"`
	IndexSize int64           `json:"indexSize"`
	Range     string          `json:"range"`
	raw       *RawChunkRecord `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimpleChunkRecord) Raw() *RawChunkRecord {
	return s.raw
}

func (s *SimpleChunkRecord) SetRaw(raw *RawChunkRecord) {
	s.raw = raw
}

func (s *SimpleChunkRecord) Model(chain, format string, verbose bool, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]any{
		"range":     s.Range,
		"bloomHash": s.BloomHash,
		"bloomSize": s.BloomSize,
		"indexHash": s.IndexHash,
		"indexSize": s.IndexSize,
	}
	order = []string{
		"range",
		"bloomHash",
		"bloomSize",
		"indexHash",
		"indexSize",
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// EXISTING_CODE
func (s *SimpleChunkRecord) String() string {
	bytes, _ := json.MarshalIndent(s, "", "  ")
	return string(bytes)
}

// EXISTING_CODE
