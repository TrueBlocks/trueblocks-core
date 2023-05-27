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

type simpleChunkPinReport struct {
	Chain        string        `json:"chain"`
	ManifestHash base.IpfsHash `json:"manifestHash"`
	NPinned      uint64        `json:"nPinned"`
	Schemas      base.IpfsHash `json:"schemas"`
	TsHash       base.IpfsHash `json:"tsHash"`
	Version      string        `json:"version"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *simpleChunkPinReport) Raw() *types.RawModeler {
	return nil
}

func (s *simpleChunkPinReport) Model(verbose bool, format string, extraOptions map[string]any) types.Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	model["chain"] = s.Chain
	model["manifestHash"] = s.ManifestHash
	model["nPinned"] = s.NPinned
	model["schemas"] = s.Schemas
	model["tsHash"] = s.TsHash
	model["version"] = s.Version
	order = append(order, "chain")
	order = append(order, "manifestHash")
	order = append(order, "nPinned")
	order = append(order, "schemas")
	order = append(order, "tsHash")
	order = append(order, "version")
	// EXISTING_CODE

	return types.Model{
		Data:  model,
		Order: order,
	}
}

// EXISTING_CODE
// EXISTING_CODE
