// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package types

// EXISTING_CODE
import (
	"io"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

// EXISTING_CODE

type RawChunkIndex struct {
	AddressCount    string `json:"addressCount"`
	AppearanceCount string `json:"appearanceCount"`
	Hash            string `json:"hash"`
	Magic           string `json:"magic"`
	Range           string `json:"range"`
	Size            string `json:"size"`
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleChunkIndex struct {
	AddressCount    uint64         `json:"addressCount"`
	AppearanceCount uint64         `json:"appearanceCount"`
	Hash            base.Hash      `json:"hash"`
	Magic           string         `json:"magic"`
	Range           base.FileRange `json:"range"`
	Size            uint64         `json:"size"`
	raw             *RawChunkIndex `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimpleChunkIndex) Raw() *RawChunkIndex {
	return s.raw
}

func (s *SimpleChunkIndex) SetRaw(raw *RawChunkIndex) {
	s.raw = raw
}

func (s *SimpleChunkIndex) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *SimpleChunkIndex) WriteTo(w io.Writer) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

func (s *SimpleChunkIndex) ReadFrom(r io.Reader) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

// EXISTING_CODE
// EXISTING_CODE
