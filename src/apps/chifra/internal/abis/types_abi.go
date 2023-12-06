// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package abisPkg

// EXISTING_CODE
import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

type CFunctionArray []types.SimpleFunction

// EXISTING_CODE

type simpleAbi struct {
	Address   base.Address   `json:"address"`
	Functions CFunctionArray `json:"functions"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *simpleAbi) Raw() *types.RawModeler {
	return nil
}

func (s *simpleAbi) Model(chain, format string, verbose bool, extraOptions map[string]any) types.Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	model[s.Address.Hex()] = s.Functions
	order = append(order, s.Address.Hex())
	// EXISTING_CODE

	return types.Model{
		Data:  model,
		Order: order,
	}
}

// EXISTING_CODE
// EXISTING_CODE
