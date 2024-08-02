// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package types

// EXISTING_CODE
import "encoding/json"

// EXISTING_CODE

type AbiFile struct {
	FileSize    int64  `json:"fileSize"`
	LastModDate string `json:"lastModDate"`
	Name        string `json:"name"`
	Path        string `json:"path"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s AbiFile) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *AbiFile) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	var model = map[string]any{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]any{
		"name":        s.Name,
		"lastModDate": s.LastModDate,
		"fileSize":    s.FileSize,
		"path":        s.Path,
	}
	order = []string{
		"name",
		"lastModDate",
		"fileSize",
		"path",
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *AbiFile) FinishUnmarshal() {
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
