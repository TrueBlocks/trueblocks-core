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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// EXISTING_CODE

type ChunkBloom struct {
	ByteWidth  uint64      `json:"byteWidth"`
	FileSize   uint64      `json:"fileSize"`
	Hash       base.Hash   `json:"hash"`
	Magic      string      `json:"magic"`
	NBlooms    uint64      `json:"nBlooms"`
	NInserted  uint64      `json:"nInserted"`
	Range      string      `json:"range"`
	RangeDates *RangeDates `json:"rangeDates,omitempty"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s ChunkBloom) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *ChunkBloom) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	props := NewModelProps(chain, format, verbose, extraOpts)

	rawNames := []Labeler{}
	model := s.RawMap(props, rawNames)
	for k, v := range s.CalcMap(props) {
		model[k] = v
	}

	var order = []string{}
	// EXISTING_CODE
	order = []string{
		"range",
		"magic",
		"hash",
		"nBlooms",
		"fileSize",
		"byteWidth",
	}

	if verbose {
		order = append(order, "nInserted")
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

// RawMap returns a map containing only the raw/base fields for this ChunkBloom.
// This excludes any calculated or derived fields.
func (s *ChunkBloom) RawMap(p *ModelProps, needed []Labeler) map[string]any {
	model := map[string]any{
		"range":     s.Range,
		"magic":     s.Magic,
		"nBlooms":   s.NBlooms,
		"fileSize":  s.FileSize,
		"byteWidth": s.ByteWidth,
	}

	if p.Verbose {
		model["nInserted"] = s.NInserted
	}

	return labelAddresses(p, model, needed)
}

// CalcMap returns a map containing the calculated/derived fields for this ChunkBloom.
// This includes hash formatting, range dates, and other computed values.
func (s *ChunkBloom) CalcMap(p *ModelProps) map[string]any {
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

	return model
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *ChunkBloom) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
func FormattedTag(verbose bool, hash base.Hash) string {
	if tag, ok := config.VersionTags[hash.Hex()]; !ok {
		return utils.FormattedHash(verbose, hash.Hex())
	} else {
		return tag
	}
}

// EXISTING_CODE
