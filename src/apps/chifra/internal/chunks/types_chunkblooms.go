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

type simpleChunkBlooms struct {
	Count     uint64         `json:"count"`
	Hash      base.Hash      `json:"hash"`
	Magic     string         `json:"magic"`
	NInserted uint64         `json:"nInserted"`
	Range     base.FileRange `json:"range"`
	Size      uint64         `json:"size"`
	Width     uint64         `json:"width"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *simpleChunkBlooms) Raw() *types.RawModeler {
	return nil
}

func (s *simpleChunkBlooms) Model(showHidden bool, format string, extraOptions map[string]any) types.Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	// EXISTING_CODE

	return types.Model{
		Data:  model,
		Order: order,
	}
}

// EXISTING_CODE
// EXISTING_CODE
