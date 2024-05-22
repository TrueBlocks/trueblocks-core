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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// EXISTING_CODE

type ChunkBloom struct {
	ByteWidth uint64    `json:"byteWidth"`
	Hash      base.Hash `json:"hash"`
	Magic     string    `json:"magic"`
	NBlooms   uint64    `json:"nBlooms"`
	NInserted uint64    `json:"nInserted"`
	Range     string    `json:"range"`
	Size      uint64    `json:"size"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s ChunkBloom) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *ChunkBloom) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]interface{}{
		"range":     s.Range,
		"magic":     s.Magic,
		"hash":      FormattedTag(verbose, s.Hash),
		"nBlooms":   s.NBlooms,
		"nInserted": s.NInserted,
		"size":      s.Size,
		"byteWidth": s.ByteWidth,
	}
	order = []string{
		"range",
		"magic",
		"hash",
		"nBlooms",
		"nInserted",
		"size",
		"byteWidth",
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
func (s *ChunkBloom) FinishUnmarshal() {
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
func FormattedTag(verbose bool, hash base.Hash) string {
	if tag, ok := config.VersionTags[hash.Hex()]; !ok {
		return utils.FormattedHash(verbose, hash.Hex())
	} else {
		return tag
	}
}

// EXISTING_CODE
