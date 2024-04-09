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

func (s *SimpleAppearanceCount) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *SimpleAppearanceCount) Raw() *RawAppearanceCount {
	return s.raw
}

func (s *SimpleAppearanceCount) SetRaw(raw *RawAppearanceCount) {
	s.raw = raw
}

func (s *SimpleAppearanceCount) Model(chain, format string, verbose bool, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *SimpleAppearanceCount) FinishUnmarshal() {
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
