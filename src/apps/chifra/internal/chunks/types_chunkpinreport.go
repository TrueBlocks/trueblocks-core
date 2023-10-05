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
	Chain         string        `json:"chain"`
	ManifestHash  base.IpfsHash `json:"manifestHash"`
	Specification base.IpfsHash `json:"specification"`
	TsHash        base.IpfsHash `json:"tsHash"`
	Version       string        `json:"version"`
	// EXISTING_CODE
	Pinned []base.IpfsHash `json:"-"`
	// EXISTING_CODE
}

func (s *simpleChunkPinReport) Raw() *types.RawModeler {
	return nil
}

func (s *simpleChunkPinReport) Model(chain, format string, verbose bool, extraOptions map[string]any) types.Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]interface{}{
		"chain":         s.Chain,
		"manifestHash":  s.ManifestHash,
		"specification": s.Specification,
		"tsHash":        s.TsHash,
		"version":       s.Version,
		"nPinned":       len(s.Pinned),
	}
	order = []string{
		"chain",
		"manifestHash",
		"specification",
		"tsHash",
		"version",
		"nPinned",
	}
	// EXISTING_CODE

	return types.Model{
		Data:  model,
		Order: order,
	}
}

// EXISTING_CODE
// EXISTING_CODE
