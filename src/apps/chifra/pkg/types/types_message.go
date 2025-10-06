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
	props := NewModelProps(chain, format, verbose, extraOpts)

	rawNames := []Labeler{}
	model := s.RawMap(props, rawNames)
	for k, v := range s.CalcMap(props) {
		model[k] = v
	}

	var order = []string{}
	// EXISTING_CODE
	if len(s.Msg) > 0 {
		order = append(order, "msg")
	}
	if s.Num > 0 {
		order = append(order, "num")
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// RawMap returns a map containing only the raw/base fields for this Message.
// This excludes any calculated or derived fields.
func (s *Message) RawMap(p *ModelProps, needed []Labeler) map[string]any {
	model := map[string]any{}

	if len(s.Msg) > 0 {
		model["msg"] = s.Msg
	}
	if s.Num > 0 {
		model["num"] = s.Num
	}

	return labelAddresses(p, model, needed)
}

// CalcMap returns a map containing only the calculated/derived fields for this Message.
// This is optimized for streaming contexts where the frontend receives the raw Message
// and needs to enhance it with calculated values.
func (s *Message) CalcMap(p *ModelProps) map[string]any {
	model := map[string]any{}

	// No calculated fields in Message

	return model
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *Message) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
