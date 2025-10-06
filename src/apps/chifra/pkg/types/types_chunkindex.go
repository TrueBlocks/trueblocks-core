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

type ChunkIndex struct {
	FileSize     uint64      `json:"fileSize"`
	Hash         base.Hash   `json:"hash"`
	Magic        string      `json:"magic"`
	NAddresses   uint64      `json:"nAddresses"`
	NAppearances uint64      `json:"nAppearances"`
	Range        string      `json:"range"`
	RangeDates   *RangeDates `json:"rangeDates,omitempty"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s ChunkIndex) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *ChunkIndex) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
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
		"magic",
		"hash",
		"nAddresses",
		"nAppearances",
		"fileSize",
	}

	if format == "json" {
		order = append(order, "hashValue")
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

// RawMap returns a map containing only the raw/base fields for this ChunkIndex.
// This excludes any calculated or derived fields.
func (s *ChunkIndex) RawMap(p *ModelProps, needed []Labeler) map[string]any {
	model := map[string]any{
		"range":        s.Range,
		"magic":        s.Magic,
		"nAddresses":   s.NAddresses,
		"nAppearances": s.NAppearances,
		"fileSize":     s.FileSize,
	}

	return labelAddresses(p, model, needed)
}

// CalcMap returns a map containing the calculated/derived fields for this ChunkIndex.
// This includes hash formatting, range dates, and other computed values.
func (s *ChunkIndex) CalcMap(p *ModelProps, needed []Labeler) map[string]any {
	model := map[string]any{
		"hash": FormattedTag(p.Verbose, s.Hash),
	}

	if p.Format == "json" {
		model["hash"] = s.Hash.Hex()
		model["hashValue"] = FormattedTag(p.Verbose, s.Hash)
	}

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
func (s *ChunkIndex) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
