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

type Timestamp struct {
	BlockNumber base.Blknum    `json:"blockNumber"`
	Diff        int64          `json:"diff"`
	Timestamp   base.Timestamp `json:"timestamp"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s Timestamp) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *Timestamp) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	props := &ModelProps{
		Chain:     chain,
		Format:    format,
		Verbose:   verbose,
		ExtraOpts: extraOpts,
	}

	rawNames := []Labeler{} // No addresses in Timestamp
	model := s.RawMap(props, rawNames)

	calcNames := []Labeler{}
	for k, v := range s.CalcMap(props, calcNames) {
		model[k] = v
	}

	var order = []string{}
	// EXISTING_CODE
	order = []string{
		"blockNumber",
		"timestamp",
		"diff",
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// RawMap returns a map containing only the raw/base fields for this Timestamp.
// This excludes any calculated or derived fields.
func (s *Timestamp) RawMap(p *ModelProps, needed []Labeler) map[string]any {
	model := map[string]any{
		"blockNumber": s.BlockNumber,
		"timestamp":   s.Timestamp,
		"diff":        s.Diff,
	}

	return labelAddresses(p, model, needed)
}

// CalcMap returns a map containing only the calculated/derived fields for this Timestamp.
// This is optimized for streaming contexts where the frontend receives the raw Timestamp
// and needs to enhance it with calculated values.
func (s *Timestamp) CalcMap(p *ModelProps, needed []Labeler) map[string]any {
	model := map[string]any{}

	// No calculated fields in original Model method

	return labelAddresses(p, model, needed)
}

func (s *Timestamp) Date() string {
	return base.FormattedDate(s.Timestamp)
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *Timestamp) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
