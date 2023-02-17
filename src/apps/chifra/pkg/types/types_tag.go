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

type RawTag struct {
	Tags string `json:"tags"`
}

type SimpleTag struct {
	Tags string `json:"tags"`
	raw  *RawTag
}

func (s *SimpleTag) Raw() *RawTag {
	return s.raw
}

func (s *SimpleTag) SetRaw(raw *RawTag) {
	s.raw = raw
}

func (s *SimpleTag) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	// EXISTING_CODE
	// EXISTING_CODE

	model := map[string]interface{}{
		"tags": s.Tags,
	}

	order := []string{
		"tags",
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
