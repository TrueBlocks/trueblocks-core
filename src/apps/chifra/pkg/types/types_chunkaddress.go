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

type RawChunkAddress struct {
	Address string `json:"address"`
	Count   string `json:"count"`
	Offset  string `json:"offset"`
	Range   string `json:"range"`
}

type ChunkAddress struct {
	Address base.Address     `json:"address"`
	Count   uint64           `json:"count"`
	Offset  uint64           `json:"offset"`
	Range   string           `json:"range"`
	raw     *RawChunkAddress `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s ChunkAddress) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *ChunkAddress) Raw() *RawChunkAddress {
	return s.raw
}

func (s *ChunkAddress) SetRaw(raw *RawChunkAddress) {
	s.raw = raw
}

func (s *ChunkAddress) Model(chain, format string, verbose bool, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]interface{}{
		"address": s.Address,
		"range":   s.Range,
		"offset":  s.Offset,
		"count":   s.Count,
	}
	order = []string{
		"address",
		"range",
		"offset",
		"count",
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *ChunkAddress) FinishUnmarshal() {
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
