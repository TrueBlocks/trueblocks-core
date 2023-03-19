// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package types

// EXISTING_CODE
import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"

// EXISTING_CODE

type RawNamedBlock struct {
	BlockNumber string `json:"blockNumber"`
	Date        string `json:"date"`
	Name        string `json:"name"`
	Timestamp   string `json:"timestamp"`
}

type SimpleNamedBlock struct {
	BlockNumber base.Blknum    `json:"blockNumber"`
	Date        string         `json:"date"`
	Name        string         `json:"name,omitempty"`
	Timestamp   base.Timestamp `json:"timestamp"`
	raw         *RawNamedBlock
}

func (s *SimpleNamedBlock) Raw() *RawNamedBlock {
	return s.raw
}

func (s *SimpleNamedBlock) SetRaw(raw *RawNamedBlock) {
	s.raw = raw
}

func (s *SimpleNamedBlock) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	// EXISTING_CODE
	// EXISTING_CODE

	model := map[string]interface{}{
		"blockNumber": s.BlockNumber,
		"date":        s.Date,
		"timestamp":   s.Timestamp,
	}

	order := []string{
		"blockNumber",
		"timestamp",
		"date",
	}

	// EXISTING_CODE
	// TODO: Can we automate omitempty?
	if format != "json" || len(s.Name) > 0 {
		model["name"] = s.Name
		order = append(order, "name")
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *SimpleNamedBlock) Write(p []byte) (n int, err error) {
	// JIMMY_JAM
	// JIMMY_JAM
	return 0, nil
}

func (s *SimpleNamedBlock) Read(p []byte) (n int, err error) {
	// JIMMY_JAM
	// JIMMY_JAM
	return 0, nil
}

// EXISTING_CODE
// EXISTING_CODE
