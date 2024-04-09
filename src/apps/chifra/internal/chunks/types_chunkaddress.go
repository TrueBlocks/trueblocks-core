// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package chunksPkg

// EXISTING_CODE
import (
	"encoding/json"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// EXISTING_CODE

type simpleChunkAddress struct {
	Address base.Address `json:"address"`
	Count   uint64       `json:"count"`
	Offset  uint64       `json:"offset"`
	Range   string       `json:"range"`

	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *simpleChunkAddress) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *simpleChunkAddress) Raw() *types.RawModeler {
	return nil
}

func (s *simpleChunkAddress) Model(chain, format string, verbose bool, extraOptions map[string]any) types.Model {
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

	return types.Model{
		Data:  model,
		Order: order,
	}
}

// EXISTING_CODE
// EXISTING_CODE
