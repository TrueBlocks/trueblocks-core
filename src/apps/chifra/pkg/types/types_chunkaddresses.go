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

type RawhunkAddresses struct {
	Address string `json:"address"`
	Count   string `json:"count"`
	Offset  string `json:"offset"`
	Range   string `json:"range"`
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimplehunkAddresses struct {
	Address base.Address      `json:"address"`
	Count   uint64            `json:"count"`
	Offset  uint64            `json:"offset"`
	Range   base.FileRange    `json:"range"`
	raw     *RawhunkAddresses `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimplehunkAddresses) Raw() *RawhunkAddresses {
	return s.raw
}

func (s *SimplehunkAddresses) SetRaw(raw *RawhunkAddresses) {
	s.raw = raw
}

func (s *SimplehunkAddresses) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *SimplehunkAddresses) WriteTo(w io.Writer) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

func (s *SimplehunkAddresses) ReadFrom(r io.Reader) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

// EXISTING_CODE
// EXISTING_CODE
