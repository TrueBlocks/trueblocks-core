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

type RawPinnedChunk struct {
	BloomHash string `json:"bloomHash"`
	FirstApp  string `json:"firstApp"`
	IndexHash string `json:"indexHash"`
	LatestApp string `json:"latestApp"`
	Range     string `json:"range"`
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimplePinnedChunk struct {
	BloomHash base.IpfsHash   `json:"bloomHash"`
	FirstApp  base.Blknum     `json:"firstApp"`
	IndexHash base.IpfsHash   `json:"indexHash"`
	LatestApp base.Blknum     `json:"latestApp"`
	Range     string          `json:"range"`
	raw       *RawPinnedChunk `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimplePinnedChunk) Raw() *RawPinnedChunk {
	return s.raw
}

func (s *SimplePinnedChunk) SetRaw(raw *RawPinnedChunk) {
	s.raw = raw
}

func (s *SimplePinnedChunk) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *SimplePinnedChunk) WriteTo(w io.Writer) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

func (s *SimplePinnedChunk) ReadFrom(r io.Reader) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

// EXISTING_CODE
// EXISTING_CODE
