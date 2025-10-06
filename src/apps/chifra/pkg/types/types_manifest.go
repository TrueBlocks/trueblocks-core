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

type ChunkManifest = Manifest

// EXISTING_CODE

type Manifest struct {
	Chain         string        `json:"chain"`
	Chunks        []ChunkRecord `json:"chunks"`
	Specification base.IpfsHash `json:"specification"`
	Version       string        `json:"version"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s Manifest) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *Manifest) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	props := NewModelProps(chain, format, verbose, extraOpts)

	rawNames := []Labeler{}
	model := s.RawMap(props, rawNames)
	for k, v := range s.CalcMap(props) {
		model[k] = v
	}

	var order = []string{}
	// EXISTING_CODE
	order = []string{
		"version",
		"chain",
		"specification",
		"chunks",
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// RawMap returns a map containing only the raw/base fields for this Manifest.
// This excludes any calculated or derived fields.
func (s *Manifest) RawMap(p *ModelProps, needed []Labeler) map[string]any {
	model := map[string]any{
		"version":       s.Version,
		"chain":         s.Chain,
		"specification": s.Specification,
		"chunks":        s.Chunks,
	}

	return labelAddresses(p, model, needed)
}

// CalcMap returns a map containing only the calculated/derived fields for this Manifest.
// This is optimized for streaming contexts where the frontend receives the raw Manifest
// and needs to enhance it with calculated values.
func (s *Manifest) CalcMap(p *ModelProps) map[string]any {
	model := map[string]any{}

	// No calculated fields in Manifest

	return model
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *Manifest) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
func (s *Manifest) ShallowCopy() Manifest {
	return Manifest{
		Chain:         s.Chain,
		Specification: s.Specification,
		Version:       s.Version,
	}
}

// EXISTING_CODE
