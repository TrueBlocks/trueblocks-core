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

type RawIndexCacheItem struct {
	BloomHash          string `json:"bloomHash"`
	BloomSizeBytes     string `json:"bloomSizeBytes"`
	FileDate           string `json:"fileDate"`
	Filename           string `json:"filename"`
	FirstApp           string `json:"firstApp"`
	FirstTs            string `json:"firstTs"`
	IndexHash          string `json:"indexHash"`
	IndexSizeBytes     string `json:"indexSizeBytes"`
	LatestApp          string `json:"latestApp"`
	LatestTs           string `json:"latestTs"`
	NAddrs             string `json:"nAddrs"`
	NApps              string `json:"nApps"`
	IndexCacheItemType string `json:"type"`
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleIndexCacheItem struct {
	BloomHash          base.IpfsHash      `json:"bloomHash"`
	BloomSizeBytes     uint32             `json:"bloomSizeBytes"`
	FileDate           string             `json:"fileDate"`
	Filename           string             `json:"filename"`
	FirstApp           base.Blknum        `json:"firstApp"`
	FirstTs            base.Timestamp     `json:"firstTs"`
	IndexHash          base.IpfsHash      `json:"indexHash"`
	IndexSizeBytes     uint32             `json:"indexSizeBytes"`
	LatestApp          base.Blknum        `json:"latestApp"`
	LatestTs           base.Timestamp     `json:"latestTs"`
	NAddrs             uint32             `json:"nAddrs"`
	NApps              uint32             `json:"nApps"`
	IndexCacheItemType string             `json:"type"`
	raw                *RawIndexCacheItem `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimpleIndexCacheItem) Raw() *RawIndexCacheItem {
	return s.raw
}

func (s *SimpleIndexCacheItem) SetRaw(raw *RawIndexCacheItem) {
	s.raw = raw
}

func (s *SimpleIndexCacheItem) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *SimpleIndexCacheItem) WriteTo(w io.Writer) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

func (s *SimpleIndexCacheItem) ReadFrom(r io.Reader) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

// EXISTING_CODE
// EXISTING_CODE
