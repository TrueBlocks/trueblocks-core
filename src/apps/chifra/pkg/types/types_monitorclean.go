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

type MonitorClean struct {
	Address  base.Address `json:"address"`
	Dups     int64        `json:"dups"`
	Removed  bool         `json:"removed"`
	SizeNow  int64        `json:"sizeNow"`
	SizeThen int64        `json:"sizeThen"`
	Staged   bool         `json:"staged"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s MonitorClean) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *MonitorClean) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	props := NewModelProps(chain, format, verbose, extraOpts)

	rawNames := []Labeler{
		NewLabeler(s.Address, "address"),
	}
	model := s.RawMap(props, &rawNames)
	for k, v := range s.CalcMap(props) {
		model[k] = v
	}

	var order = []string{}
	// EXISTING_CODE
	order = []string{
		"address",
		"sizeNow",
		"sizeThen",
		"dups",
	}

	if extraOpts["staged"] == true {
		order = append(order, "staged", "removed")
	}
	// EXISTING_CODE

	for _, item := range rawNames {
		key := item.name + "Name"
		if _, exists := model[key]; exists {
			order = append(order, key)
		}
	}
	order = reorderFields(order)

	return Model{
		Data:  model,
		Order: order,
	}
}

// RawMap returns a map containing only the raw/base fields for this MonitorClean.
func (s *MonitorClean) RawMap(p *ModelProps, needed *[]Labeler) map[string]any {
	model := map[string]any{
		// EXISTING_CODE
		"address":  s.Address,
		"sizeNow":  s.SizeNow,
		"sizeThen": s.SizeThen,
		"dups":     s.Dups,
		// EXISTING_CODE
	}

	// EXISTING_CODE
	// EXISTING_CODE

	return labelAddresses(p, model, needed)
}

// CalcMap returns a map containing the calculated/derived fields for this MonitorClean.
func (s *MonitorClean) CalcMap(p *ModelProps) map[string]any {
	model := map[string]any{
		// EXISTING_CODE
		// EXISTING_CODE
	}

	// EXISTING_CODE
	if p.ExtraOpts["staged"] == true {
		model["staged"] = s.Staged
		model["removed"] = s.Removed
	}
	// EXISTING_CODE

	return model
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *MonitorClean) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
