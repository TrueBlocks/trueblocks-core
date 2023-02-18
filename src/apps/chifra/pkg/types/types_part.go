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

type RawPart struct {
	Tags    string `json:"tags"`
	Address string `json:"address"`
}

type SimplePart struct {
	Tags    string  `json:"tags,omitempty"`
	Address Address `json:"address,omitempty"`
	raw     *RawPart
}

func (s *SimplePart) Raw() *RawPart {
	return s.raw
}

func (s *SimplePart) SetRaw(raw *RawPart) {
	s.raw = raw
}

func (s *SimplePart) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	// EXISTING_CODE
	// EXISTING_CODE

	model := map[string]interface{}{
	}

	order := []string{
	}

	// EXISTING_CODE
	if extraOptions["tags"] == true {
		model["tags"] = s.Tags
		order = append(order, "tags")
	} else if extraOptions["address"] == true {
		model["address"] = s.Address.Hex()
		order = append(order, "address")
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// EXISTING_CODE
// EXISTING_CODE
