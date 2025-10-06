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
)

// EXISTING_CODE

type Count struct {
	Count uint64 `json:"count"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s Count) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *Count) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	props := &ModelProps{
		Chain:     chain,
		Format:    format,
		Verbose:   verbose,
		ExtraOpts: extraOpts,
	}

	rawNames := []Labeler{}
	model := s.RawMap(props, rawNames)

	calcNames := []Labeler{}
	for k, v := range s.CalcMap(props, calcNames) {
		model[k] = v
	}

	var order = []string{}
	// EXISTING_CODE
	order = []string{
		"count",
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// RawMap returns a map containing only the raw/base fields for this Count.
// This excludes any calculated or derived fields.
func (s *Count) RawMap(p *ModelProps, needed []Labeler) map[string]any {
	model := map[string]any{
		"count": s.Count,
	}

	return labelAddresses(p, model, needed)
}

// CalcMap returns a map containing the calculated/derived fields for this Count.
// This type has no calculated fields currently.
func (s *Count) CalcMap(p *ModelProps, needed []Labeler) map[string]any {
	model := map[string]any{}

	return labelAddresses(p, model, needed)
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *Count) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
