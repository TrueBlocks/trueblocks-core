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
	ByteWidth  uint64           `json:"byteWidth"`
	FileSize   uint64           `json:"fileSize"`
	Hash       base.Hash        `json:"hash"`
	Magic      string           `json:"magic"`
	NBlooms    uint64           `json:"nBlooms"`
	NInserted  uint64           `json:"nInserted"`
	Range      string           `json:"range"`
	RangeDates *RangeDates      `json:"rangeDates,omitempty"`
	Calcs      *ChunkBloomCalcs `json:"calcs,omitempty"`
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
	model := s.RawMap(props, &rawNames)
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

// RawMap returns a map containing only the raw/base fields for this ChunkBloom.
func (s *ChunkBloom) RawMap(p *ModelProps, needed *[]Labeler) map[string]any {
	model := map[string]any{
		// EXISTING_CODE
		"range":     s.Range,
		"magic":     s.Magic,
		"nBlooms":   s.NBlooms,
		"fileSize":  s.FileSize,
		"byteWidth": s.ByteWidth,
		// EXISTING_CODE
	}

	// EXISTING_CODE
	if p.Verbose {
		model["nInserted"] = s.NInserted
	}
	// EXISTING_CODE

	return labelAddresses(p, model, needed)
}

// CalcMap returns a map containing the calculated/derived fields for this type.
func (s *ChunkBloom) CalcMap(p *ModelProps) map[string]any {
	model := map[string]any{
		// EXISTING_CODE
		"hash": FormattedTag(p.Verbose, s.Hash),
		// EXISTING_CODE
	}

	// EXISTING_CODE
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
	// EXISTING_CODE

	return model
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *ChunkBloom) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	s.Calcs = nil
	// EXISTING_CODE
	// EXISTING_CODE
}

// ChunkBloomCalcs holds lazy-loaded calculated fields for ChunkBloom
type ChunkBloomCalcs struct {
	// EXISTING_CODE
	Hash       string         `json:"hash"`
	HashValue  string         `json:"hashValue,omitempty"`
	RangeDates interface{}    `json:"rangeDates,omitempty"`
	FirstTs    base.Timestamp `json:"firstTs,omitempty"`
	FirstDate  string         `json:"firstDate,omitempty"`
	LastTs     base.Timestamp `json:"lastTs,omitempty"`
	LastDate   string         `json:"lastDate,omitempty"`
	// EXISTING_CODE
}

func (s *ChunkBloom) EnsureCalcs(p *ModelProps, requestedFields []string) error {
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

	s.Calcs = &ChunkBloomCalcs{}
	return json.Unmarshal(jsonBytes, s.Calcs)
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
