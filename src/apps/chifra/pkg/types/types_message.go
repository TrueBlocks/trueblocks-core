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

type Message struct {
	Msg string `json:"msg,omitempty"`
	Num int64  `json:"num,omitempty"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s Message) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *Message) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	var model = map[string]any{}
	var order = []string{}

	// EXISTING_CODE
	if len(s.Msg) > 0 {
		model["msg"] = s.Msg
		order = append(order, "msg")
	}
	if s.Num > 0 {
		model["num"] = s.Num
		order = append(order, "num")
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *Message) FinishUnmarshal(fileVersion uint64) {
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
