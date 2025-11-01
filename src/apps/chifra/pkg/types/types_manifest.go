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

type ChunkManifest = Manifest

// EXISTING_CODE

type Manifest struct {
	Chain         string         `json:"chain"`
	Chunks        []ChunkRecord  `json:"chunks"`
	Specification base.IpfsHash  `json:"specification"`
	Version       string         `json:"version"`
	Calcs         *ManifestCalcs `json:"calcs,omitempty"`
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
	model := s.RawMap(props, &rawNames)
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

// RawMap returns a map containing only the raw/base fields for this Manifest.
func (s *Manifest) RawMap(p *ModelProps, needed *[]Labeler) map[string]any {
	model := map[string]any{
		// EXISTING_CODE
		"version":       s.Version,
		"chain":         s.Chain,
		"specification": s.Specification,
		"chunks":        s.Chunks,
		// EXISTING_CODE
	}

	// EXISTING_CODE
	// EXISTING_CODE

	return labelAddresses(p, model, needed)
}

// CalcMap returns a map containing the calculated/derived fields for this type.
func (s *Manifest) CalcMap(p *ModelProps) map[string]any {
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
func (s *Manifest) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	s.Calcs = nil
	// EXISTING_CODE
	// EXISTING_CODE
}

// ManifestCalcs holds lazy-loaded calculated fields for Manifest
type ManifestCalcs struct {
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *Manifest) EnsureCalcs(p *ModelProps, fieldFilter []string) error {
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

	s.Calcs = &ManifestCalcs{}
	return json.Unmarshal(jsonBytes, s.Calcs)
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
