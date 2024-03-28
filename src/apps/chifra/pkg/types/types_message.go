// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package types

// EXISTING_CODE

// EXISTING_CODE

type SimpleMessage struct {
	Msg string `json:"msg,omitempty"`
	Num int    `json:"num,omitempty"`
}

func (s *SimpleMessage) Raw() *RawModeler {
	return nil
}

func (s *SimpleMessage) Model(chain, format string, verbose bool, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
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
func (s *SimpleMessage) FinishUnmarshal() {
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
