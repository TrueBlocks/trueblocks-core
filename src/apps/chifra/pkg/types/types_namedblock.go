// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package types

// EXISTING_CODE
// EXISTING_CODE

type RawNamedBlock interface{}

type SimpleNamedBlock struct {
	BlockNumber uint64 `json:"blockNumber"`
	Timestamp uint64 `json:"timestamp"`
	Date string `json:"date"`
	Name string `json:"name,omitempty"`
}

func (s *SimpleNamedBlock) Raw() *RawNamedBlock {
	return nil
}

func (s *SimpleNamedBlock) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	model := map[string]interface{}{
		"blockNumber": s.BlockNumber,
		"timestamp": s.Timestamp,
		"date": s.Date,
	}

	order := []string{
		"blockNumber",
		"timestamp",
		"date",
	}

	// EXISTING_CODE
	// TODO: Can we automate omitempty?
	// omitempty
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

// EXISTING_CODE
func (s *SimpleNamedBlock) GetTimestamp() uint64 {
	return s.Timestamp
}

// EXISTING_CODE