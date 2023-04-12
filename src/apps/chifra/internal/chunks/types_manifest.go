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

type simpleManifest struct {
	Chain     string                    `json:"chain"`
	Chunks    []types.SimpleChunkRecord `json:"chunks"`
	Databases base.IpfsHash             `json:"databases"`
	Schemas   base.IpfsHash             `json:"schemas"`
	Version   string                    `json:"version"`
	// EXISTING_CODE
	// Databases base.IpfsHash       `json:"databases"`
	// EXISTING_CODE
}

func (s *simpleManifest) Raw() *types.RawModeler {
	return nil
}

func (s *simpleManifest) Model(showHidden bool, format string, extraOptions map[string]any) types.Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]any{
		"version": s.Version,
		"chain":   s.Chain,
		"schemas": s.Schemas,
		"chunks":  s.Chunks,
	}
	order = []string{
		"version",
		"chain",
		"schemas",
		"chunks",
	}
	// EXISTING_CODE

	return types.Model{
		Data:  model,
		Order: order,
	}
}

// EXISTING_CODE
// EXISTING_CODE
