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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/base"
)

// EXISTING_CODE

type RangeDates struct {
	FirstDate string           `json:"firstDate,omitempty"`
	FirstTs   base.Timestamp   `json:"firstTs,omitempty"`
	LastDate  string           `json:"lastDate,omitempty"`
	LastTs    base.Timestamp   `json:"lastTs,omitempty"`
	Calcs     *RangeDatesCalcs `json:"calcs,omitempty"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s RangeDates) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *RangeDates) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	props := NewModelProps(chain, format, verbose, extraOpts)

	rawNames := []Labeler{}
	model := s.RawMap(props, &rawNames)
	for k, v := range s.CalcMap(props) {
		model[k] = v
	}

	var order = []string{}
	// EXISTING_CODE
	order = []string{
		"firstDate",
		"firstTs",
		"lastDate",
		"lastTs",
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

// RawMap returns a map containing only the raw/base fields for this RangeDates.
func (s *RangeDates) RawMap(p *ModelProps, needed *[]Labeler) map[string]any {
	model := map[string]any{
		// EXISTING_CODE
		"firstDate": s.FirstDate,
		"firstTs":   s.FirstTs,
		"lastDate":  s.LastDate,
		"lastTs":    s.LastTs,
		// EXISTING_CODE
	}

	// EXISTING_CODE
	// EXISTING_CODE

	return labelAddresses(p, model, needed)
}

// CalcMap returns a map containing the calculated/derived fields for this type.
func (s *RangeDates) CalcMap(p *ModelProps) map[string]any {
	_ = p // delint
	model := map[string]any{
		// EXISTING_CODE
		// EXISTING_CODE
	}

	// EXISTING_CODE
	// EXISTING_CODE

	return model
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *RangeDates) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	s.Calcs = nil
	// EXISTING_CODE
	// EXISTING_CODE
}

// RangeDatesCalcs holds lazy-loaded calculated fields for RangeDates
type RangeDatesCalcs struct {
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *RangeDates) EnsureCalcs(p *ModelProps, fieldFilter []string) error {
	_ = fieldFilter // delint
	if s.Calcs != nil {
		return nil
	}

	calcMap := s.CalcMap(p)
	if len(calcMap) == 0 {
		return nil
	}

	jsonBytes, err := json.Marshal(calcMap)
	if err != nil {
		return err
	}

	s.Calcs = &RangeDatesCalcs{}
	return json.Unmarshal(jsonBytes, s.Calcs)
}

// EXISTING_CODE
func (p1 *RangeDates) Cmp(p2 RangeDates) int64 {
	return p1.FirstTs.Int64() - p2.FirstTs.Int64()
}

// EXISTING_CODE
