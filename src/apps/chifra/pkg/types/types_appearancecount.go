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

type RawAppearanceCount struct {
	Address  string `json:"address"`
	FileSize string `json:"fileSize"`
	NRecords string `json:"nRecords"`
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleAppearanceCount struct {
	Address  base.Address        `json:"address"`
	FileSize uint64              `json:"fileSize"`
	NRecords uint64              `json:"nRecords"`
	raw      *RawAppearanceCount `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimpleAppearanceCount) Raw() *RawAppearanceCount {
	return s.raw
}

func (s *SimpleAppearanceCount) SetRaw(raw *RawAppearanceCount) {
	s.raw = raw
}

func (s *SimpleAppearanceCount) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]interface{}{
		"address":  s.Address,
		"fileSize": s.FileSize,
		"nRecords": s.NRecords,
	}

	order = []string{
		"address",
		"nRecords",
		"fileSize",
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *SimpleAppearanceCount) Write(p []byte) (n int, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

func (s *SimpleAppearanceCount) Read(p []byte) (n int, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

// EXISTING_CODE
// EXISTING_CODE
