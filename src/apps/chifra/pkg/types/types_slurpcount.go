// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package types

// EXISTING_CODE
import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"

// EXISTING_CODE

type RawSlurpCount struct {
	Address string `json:"address"`
	Count   string `json:"count"`
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleSlurpCount struct {
	Address base.Address   `json:"address,omitempty"`
	Count   uint64         `json:"count,omitempty"`
	raw     *RawSlurpCount `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimpleSlurpCount) Raw() *RawSlurpCount {
	return s.raw
}

func (s *SimpleSlurpCount) SetRaw(raw *RawSlurpCount) {
	s.raw = raw
}

func (s *SimpleSlurpCount) Model(chain, format string, verbose bool, extraOptions map[string]any) Model {
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
func (s *SimpleSlurpCount) FinishUnmarshal() {
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
