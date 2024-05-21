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

type ChunkRecord struct {
	BloomHash base.IpfsHash `json:"bloomHash"`
	BloomSize int64         `json:"bloomSize"`
	IndexHash base.IpfsHash `json:"indexHash"`
	IndexSize int64         `json:"indexSize"`
	Range     string        `json:"range"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s ChunkRecord) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *ChunkRecord) Model(chain, format string, verbose bool, extraOptions map[string]any) Model {
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

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *ChunkRecord) FinishUnmarshal() {
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
