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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

// EXISTING_CODE

type RawManifest struct {
	Chain         string `json:"chain"`
	Chunks        string `json:"chunks"`
	Specification string `json:"specification"`
	Version       string `json:"version"`
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleManifest struct {
	Chain         string              `json:"chain"`
	Chunks        []SimpleChunkRecord `json:"chunks"`
	Specification base.IpfsHash       `json:"specification"`
	Version       string              `json:"version"`
	raw           *RawManifest        `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimpleManifest) Raw() *RawManifest {
	return s.raw
}

func (s *SimpleManifest) SetRaw(raw *RawManifest) {
	s.raw = raw
}

func (s *SimpleManifest) Model(chain, format string, verbose bool, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]any{
		"version":       s.Version,
		"chain":         s.Chain,
		"specification": s.Specification,
		"chunks":        s.Chunks,
	}
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

// EXISTING_CODE
// EXISTING_CODE
