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

type RawTimestampCount struct {
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleTimestampCount struct {
	Count uint64 `json:"count"`

	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimpleTimestampCount) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *SimpleTimestampCount) Raw() *RawModeler {
	return nil
}

func (s *SimpleTimestampCount) Model(chain, format string, verbose bool, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]interface{}{
		"count": s.Count,
	}
	order = []string{
		"count",
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// EXISTING_CODE
// EXISTING_CODE

// EXISTING_CODE
// EXISTING_CODE
