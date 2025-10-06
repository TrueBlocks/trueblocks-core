// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package types

// EXISTING_CODE
import "encoding/json"

// EXISTING_CODE

type ChunkStats struct {
	AddrsPerBlock float64     `json:"addrsPerBlock"`
	AppsPerAddr   float64     `json:"appsPerAddr"`
	AppsPerBlock  float64     `json:"appsPerBlock"`
	BloomSz       uint64      `json:"bloomSz"`
	ChunkSz       uint64      `json:"chunkSz"`
	NAddrs        uint64      `json:"nAddrs"`
	NApps         uint64      `json:"nApps"`
	NBlocks       uint64      `json:"nBlocks"`
	NBlooms       uint64      `json:"nBlooms"`
	Range         string      `json:"range"`
	RangeDates    *RangeDates `json:"rangeDates,omitempty"`
	Ratio         float64     `json:"ratio"`
	RecWid        uint64      `json:"recWid"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s ChunkStats) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *ChunkStats) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
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
		"nAddrs",
		"nApps",
		"nBlocks",
		"nBlooms",
		"recWid",
		"bloomSz",
		"chunkSz",
		"addrsPerBlock",
		"appsPerBlock",
		"appsPerAddr",
		"ratio",
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

// RawMap returns a map containing only the raw/base fields for this ChunkStats.
// This excludes any calculated or derived fields.
func (s *ChunkStats) RawMap(p *ModelProps, needed []Labeler) map[string]any {
	model := map[string]any{
		"addrsPerBlock": s.AddrsPerBlock,
		"appsPerAddr":   s.AppsPerAddr,
		"appsPerBlock":  s.AppsPerBlock,
		"bloomSz":       s.BloomSz,
		"chunkSz":       s.ChunkSz,
		"range":         s.Range,
		"nAddrs":        s.NAddrs,
		"nApps":         s.NApps,
		"nBlocks":       s.NBlocks,
		"nBlooms":       s.NBlooms,
		"ratio":         s.Ratio,
		"recWid":        s.RecWid,
	}

	return labelAddresses(p, model, needed)
}

// CalcMap returns a map containing the calculated/derived fields for this ChunkStats.
// This includes range date formatting and other computed values.
func (s *ChunkStats) CalcMap(p *ModelProps) map[string]any {
	model := map[string]any{}

	if p.Verbose && p.Format == "json" {
		if s.RangeDates != nil {
			model["rangeDates"] = s.RangeDates.Model(p.Chain, p.Format, p.Verbose, p.ExtraOpts).Data
		}
	} else if p.Verbose {
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
func (s *ChunkStats) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
func (s *ChunkStats) ShallowCopy() ChunkStats {
	return *s
}

// EXISTING_CODE
