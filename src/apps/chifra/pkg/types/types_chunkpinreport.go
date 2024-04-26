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

type RawChunkPinReport struct {
	Chain         string `json:"chain"`
	ManifestHash  string `json:"manifestHash"`
	SpecHash      string `json:"specHash"`
	TimestampHash string `json:"timestampHash"`
	Version       string `json:"version"`
	// EXISTING_CODE
	// EXISTING_CODE
}

type ChunkPinReport struct {
	Chain         string             `json:"chain"`
	ManifestHash  base.IpfsHash      `json:"manifestHash"`
	SpecHash      base.IpfsHash      `json:"specHash"`
	TimestampHash base.IpfsHash      `json:"timestampHash"`
	Version       string             `json:"version"`
	raw           *RawChunkPinReport `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s ChunkPinReport) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *ChunkPinReport) Raw() *RawChunkPinReport {
	return s.raw
}

func (s *ChunkPinReport) SetRaw(raw *RawChunkPinReport) {
	s.raw = raw
}

func (s *ChunkPinReport) Model(chain, format string, verbose bool, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]interface{}{
		"chain":         s.Chain,
		"version":       s.Version,
		"manifestHash":  s.ManifestHash,
		"timestampHash": s.TimestampHash,
		"specHash":      s.SpecHash,
	}
	order = []string{
		"chain",
		"version",
		"manifestHash",
		"timestampHash",
		"specHash",
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *ChunkPinReport) FinishUnmarshal() {
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
