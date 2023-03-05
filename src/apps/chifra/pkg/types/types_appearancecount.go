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

type RawAppearanceCount struct {
	Address  string `json:"address"`
	FileSize string `json:"fileSize"`
	NRecords string `json:"nRecords"`
}

type SimpleAppearanceCount struct {
	Address  Address `json:"address"`
	FileSize uint64  `json:"fileSize"`
	NRecords uint64  `json:"nRecords"`
	raw      *RawAppearanceCount
}

func (s *SimpleAppearanceCount) Raw() *RawAppearanceCount {
	return s.raw
}

func (s *SimpleAppearanceCount) SetRaw(raw *RawAppearanceCount) {
	s.raw = raw
}

func (s *SimpleAppearanceCount) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	// EXISTING_CODE
	// EXISTING_CODE

	model := map[string]interface{}{
		"address":  s.Address,
		"fileSize": s.FileSize,
		"nRecords": s.NRecords,
	}

	order := []string{
		"address",
		"nRecords",
		"fileSize",
	}

	// EXISTING_CODE
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// EXISTING_CODE
// EXISTING_CODE
