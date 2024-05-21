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

type ChunkIndex struct {
	Hash         base.Hash `json:"hash"`
	Magic        string    `json:"magic"`
	NAddresses   uint64    `json:"nAddresses"`
	NAppearances uint64    `json:"nAppearances"`
	Range        string    `json:"range"`
	Size         uint64    `json:"size"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s ChunkIndex) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *ChunkIndex) Model(chain, format string, verbose bool, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]interface{}{
		"range":        s.Range,
		"magic":        s.Magic,
		"hash":         FormattedTag(verbose, s.Hash),
		"nAddresses":   s.NAddresses,
		"nAppearances": s.NAppearances,
		"fileSize":     s.Size,
	}
	order = []string{
		"range",
		"magic",
		"hash",
		"nAddresses",
		"nAppearances",
		"fileSize",
	}
	if format == "json" {
		model["hash"] = s.Hash.Hex()
		model["hashValue"] = FormattedTag(verbose, s.Hash)
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *ChunkIndex) FinishUnmarshal() {
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
