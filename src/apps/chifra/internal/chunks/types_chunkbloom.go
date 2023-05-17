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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// EXISTING_CODE

type simpleChunkBloom struct {
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

func (s *simpleChunkBloom) Raw() *types.RawModeler {
	return nil
}

func (s *simpleChunkBloom) Model(verbose bool, format string, extraOptions map[string]any) types.Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]interface{}{
		"range":     s.Range,
		"magic":     s.Magic,
		"hash":      s.Hash,
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
	// EXISTING_CODE

	return types.Model{
		Data:  model,
		Order: order,
	}
}

// EXISTING_CODE
// EXISTING_CODE
