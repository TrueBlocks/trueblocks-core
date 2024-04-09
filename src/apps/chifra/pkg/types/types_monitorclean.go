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
)

// EXISTING_CODE

type RawMonitorClean struct {
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleMonitorClean struct {
	Address  base.Address `json:"address"`
	Dups     int64        `json:"dups"`
	SizeNow  int64        `json:"sizeNow"`
	SizeThen int64        `json:"sizeThen"`

	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimpleMonitorClean) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *SimpleMonitorClean) Raw() *RawModeler {
	return nil
}

func (s *SimpleMonitorClean) Model(chain, format string, verbose bool, extraOptions map[string]any) Model {
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

	return Model{
		Data:  model,
		Order: order,
	}
}

// EXISTING_CODE
// EXISTING_CODE

// EXISTING_CODE
// EXISTING_CODE
