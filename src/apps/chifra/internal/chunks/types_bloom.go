// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// TODO: BOGUS2 - MUST DOCUMENT
type simpleChunkBloom struct {
	Range     base.FileRange `json:"range"`
	Magic     uint16         `json:"magic"`
	Hash      base.Hash      `json:"hash"`
	Count     uint32         `json:"nBlooms"`
	NInserted uint64         `json:"nInserted"`
	Size      int64          `json:"size"`
	Width     uint64         `json:"byteWidth"`
}

func (s *simpleChunkBloom) Raw() *types.RawModeler {
	return nil
}

func (s *simpleChunkBloom) Model(showHidden bool, format string, extraOptions map[string]any) types.Model {
	return types.Model{
		Data: map[string]any{
			"range":     s.Range,
			"magic":     fmt.Sprintf("0x%x", s.Magic),
			"hash":      s.Hash,
			"nBlooms":   s.Count,
			"nInserted": s.NInserted,
			"size":      s.Size,
			"byteWidth": s.Width,
		},
		Order: []string{
			"range",
			"magic",
			"hash",
			"nBlooms",
			"nInserted",
			"size",
			"byteWidth",
		},
	}
}
