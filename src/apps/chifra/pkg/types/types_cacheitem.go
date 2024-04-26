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
)

// EXISTING_CODE

type RawCacheItem struct {
	Items         []string `json:"items"`
	LastCached    string   `json:"lastCached"`
	NFiles        string   `json:"nFiles"`
	NFolders      string   `json:"nFolders"`
	Path          string   `json:"path"`
	SizeInBytes   string   `json:"sizeInBytes"`
	CacheItemType string   `json:"type"`
	// EXISTING_CODE
	// EXISTING_CODE
}

type CacheItem struct {
	Items         []any         `json:"items"`
	LastCached    string        `json:"lastCached,omitempty"`
	NFiles        uint64        `json:"nFiles"`
	NFolders      uint64        `json:"nFolders"`
	Path          string        `json:"path"`
	SizeInBytes   int64         `json:"sizeInBytes"`
	CacheItemType string        `json:"type"`
	raw           *RawCacheItem `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s CacheItem) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *CacheItem) Raw() *RawCacheItem {
	return s.raw
}

func (s *CacheItem) SetRaw(raw *RawCacheItem) {
	s.raw = raw
}

func (s *CacheItem) Model(chain, format string, verbose bool, extraOptions map[string]any) Model {
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
func (s *CacheItem) FinishUnmarshal() {
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
