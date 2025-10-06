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

type CacheItem struct {
	Items         []any  `json:"items"`
	LastCached    string `json:"lastCached,omitempty"`
	NFiles        uint64 `json:"nFiles"`
	NFolders      uint64 `json:"nFolders"`
	Path          string `json:"path"`
	SizeInBytes   int64  `json:"sizeInBytes"`
	CacheItemType string `json:"type"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s CacheItem) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *CacheItem) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	props := NewModelProps(chain, format, verbose, extraOpts)

	rawNames := []Labeler{}
	model := s.RawMap(props, rawNames)
	for k, v := range s.CalcMap(props) {
		model[k] = v
	}

	var order = []string{}
	// EXISTING_CODE
	order = []string{
		"type",
		"path",
		"nFiles",
		"nFolders",
		"sizeInBytes",
		"lastCached",
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// RawMap returns a map containing only the raw/base fields for this CacheItem.
// This excludes any calculated or derived fields.
func (s *CacheItem) RawMap(p *ModelProps, needed []Labeler) map[string]any {
	model := map[string]any{
		"type":        s.CacheItemType,
		"path":        s.Path,
		"nFiles":      s.NFiles,
		"nFolders":    s.NFolders,
		"sizeInBytes": s.SizeInBytes,
		"lastCached":  s.LastCached,
	}

	return labelAddresses(p, model, needed)
}

// CalcMap returns a map containing the calculated/derived fields for this CacheItem.
// This type has no calculated fields currently.
func (s *CacheItem) CalcMap(p *ModelProps) map[string]any {
	model := map[string]any{}

	return model
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *CacheItem) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
