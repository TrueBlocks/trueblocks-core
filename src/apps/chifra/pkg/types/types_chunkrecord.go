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

type ChunkRecord struct {
	BloomHash  base.IpfsHash `json:"bloomHash"`
	BloomSize  int64         `json:"bloomSize"`
	IndexHash  base.IpfsHash `json:"indexHash"`
	IndexSize  int64         `json:"indexSize"`
	Range      string        `json:"range"`
	RangeDates *RangeDates   `json:"rangeDates,omitempty"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s ChunkRecord) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *ChunkRecord) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
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
		"range",
		"bloomHash",
		"bloomSize",
		"indexHash",
		"indexSize",
	}

	if verbose && format == "json" {
		if s.RangeDates != nil {
			order = append(order, "rangeDates")
		}
	} else if verbose {
		order = append(order, []string{"firstTs", "firstDate", "lastTs", "lastDate"}...)
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// RawMap returns a map containing only the raw/base fields for this ChunkRecord.
// This excludes any calculated or derived fields.
func (s *ChunkRecord) RawMap(p *ModelProps, needed []Labeler) map[string]any {
	model := map[string]any{
		"range":     s.Range,
		"bloomHash": s.BloomHash,
		"bloomSize": s.BloomSize,
		"indexHash": s.IndexHash,
		"indexSize": s.IndexSize,
	}

	return labelAddresses(p, model, needed)
}

// CalcMap returns a map containing the calculated/derived fields for this ChunkRecord.
// This includes range date formatting and other computed values.
func (s *ChunkRecord) CalcMap(p *ModelProps, needed []Labeler) map[string]any {
	model := map[string]any{}

	if p.Verbose && p.Format == "json" {
		if s.RangeDates != nil {
			model["rangeDates"] = s.RangeDates.Model(p.Chain, p.Format, p.Verbose, p.ExtraOpts).Data
		}
	} else if p.Verbose {
		model["firstTs"], model["firstDate"], model["lastTs"], model["lastDate"] = 0, "", 0, ""
		if s.RangeDates != nil {
			model["firstTs"] = s.RangeDates.FirstTs
			model["firstDate"] = s.RangeDates.FirstDate
			model["lastTs"] = s.RangeDates.LastTs
			model["lastDate"] = s.RangeDates.LastDate
		}
	}

	return labelAddresses(p, model, needed)
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *ChunkRecord) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
