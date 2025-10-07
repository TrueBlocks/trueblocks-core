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
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

// EXISTING_CODE

type Bounds struct {
	Count     uint64         `json:"count"`
	FirstApp  Appearance     `json:"firstApp"`
	FirstTs   base.Timestamp `json:"firstTs"`
	LatestApp Appearance     `json:"latestApp"`
	LatestTs  base.Timestamp `json:"latestTs"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s Bounds) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *Bounds) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	props := NewModelProps(chain, format, verbose, extraOpts)

	rawNames := []Labeler{}
	model := s.RawMap(props, &rawNames)
	for k, v := range s.CalcMap(props) {
		model[k] = v
	}

	var order = []string{}
	// EXISTING_CODE
	order = []string{
		"address",
		"count",
		"firstApp",
		"firstTs",
		"firstDate",
		"latestApp",
		"latestTs",
		"latestDate",
		"blockSpan",
		"blockFreq",
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

// RawMap returns a map containing only the raw/base fields for this Bounds.
func (s *Bounds) RawMap(p *ModelProps, needed *[]Labeler) map[string]any {
	model := map[string]any{
		// EXISTING_CODE
		"address":  s.FirstApp.Address,
		"count":    s.Count,
		"firstTs":  s.FirstTs,
		"latestTs": s.LatestTs,
		// EXISTING_CODE
	}

	// EXISTING_CODE
	if p.Format == "txt" || p.Format == "csv" {
		model["firstApp"] = fmt.Sprintf("%d.%d", s.FirstApp.BlockNumber, s.FirstApp.TransactionIndex)
		model["latestApp"] = fmt.Sprintf("%d.%d", s.LatestApp.BlockNumber, s.LatestApp.TransactionIndex)
	} else {
		model["firstApp"] = s.FirstApp.Model(p.Chain, p.Format, p.Verbose, p.ExtraOpts).Data
		model["latestApp"] = s.LatestApp.Model(p.Chain, p.Format, p.Verbose, p.ExtraOpts).Data
	}
	// EXISTING_CODE

	return labelAddresses(p, model, needed)
}

// CalcMap calculated fields:
// - firstDate (string)
// - latestDate (string)
// - blockSpan (uint64)
// - blockFreq (uint64)
func (s *Bounds) CalcMap(p *ModelProps) map[string]any {
	model := map[string]any{
		// EXISTING_CODE
		"firstDate":  base.FormattedDate(s.FirstTs),
		"latestDate": base.FormattedDate(s.LatestTs),
		"blockSpan":  (s.LatestApp.BlockNumber - s.FirstApp.BlockNumber),
		"blockFreq":  uint64(s.LatestApp.BlockNumber-s.FirstApp.BlockNumber) / s.Count,
		// EXISTING_CODE
	}

	// EXISTING_CODE
	// EXISTING_CODE

	return model
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *Bounds) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
