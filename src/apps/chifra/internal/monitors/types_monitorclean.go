// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package monitorsPkg

// EXISTING_CODE
import (
	"encoding/json"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// EXISTING_CODE

type simpleMonitorClean struct {
	Address  base.Address `json:"address"`
	Dups     int64        `json:"dups"`
	SizeNow  int64        `json:"sizeNow"`
	SizeThen int64        `json:"sizeThen"`

	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *simpleMonitorClean) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *simpleMonitorClean) Raw() *types.RawModeler {
	return nil
}

func (s *simpleMonitorClean) Model(chain, format string, verbose bool, extraOptions map[string]any) types.Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]any{
		"address":  s.Address,
		"sizeNow":  s.SizeNow,
		"sizeThen": s.SizeThen,
		"dups":     s.Dups,
	}
	order = []string{
		"address",
		"sizeNow",
		"sizeThen",
		"dups",
	}
	// EXISTING_CODE

	return types.Model{
		Data:  model,
		Order: order,
	}
}

// EXISTING_CODE
// EXISTING_CODE
