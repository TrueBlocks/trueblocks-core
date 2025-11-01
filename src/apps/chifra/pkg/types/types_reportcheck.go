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

type ReportCheck struct {
	CheckedCnt uint64            `json:"checkedCnt"`
	FailedCnt  uint64            `json:"failedCnt"`
	MsgStrings []string          `json:"msgStrings"`
	PassedCnt  uint64            `json:"passedCnt"`
	Reason     string            `json:"reason"`
	Result     string            `json:"result"`
	SkippedCnt uint64            `json:"skippedCnt"`
	VisitedCnt uint64            `json:"visitedCnt"`
	Calcs      *ReportCheckCalcs `json:"calcs,omitempty"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s ReportCheck) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *ReportCheck) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	props := NewModelProps(chain, format, verbose, extraOpts)

	rawNames := []Labeler{}
	model := s.RawMap(props, &rawNames)
	for k, v := range s.CalcMap(props) {
		model[k] = v
	}

	var order = []string{}
	// EXISTING_CODE
	order = []string{
		"result",
		"checkedCnt",
		"visitedCnt",
		"passedCnt",
	}

	if format != "json" {
		order = append(order, []string{"skippedCnt", "failedCnt", "reason"}...)
	}
	// EXISTING_CODE

	for _, item := range rawNames {
		key := item.name + "Name"
		if _, exists := model[key]; exists {
			order = append(order, key)
		}
	}
	order = reorderFields(order)

	return Model{
		Data:  model,
		Order: order,
	}
}

// RawMap returns a map containing only the raw/base fields for this ReportCheck.
func (s *ReportCheck) RawMap(p *ModelProps, needed *[]Labeler) map[string]any {
	model := map[string]any{
		// EXISTING_CODE
		"result":     s.Result,
		"checkedCnt": s.CheckedCnt,
		"visitedCnt": s.VisitedCnt,
		"passedCnt":  s.PassedCnt,
		"reason":     s.Reason,
		// EXISTING_CODE
	}

	// EXISTING_CODE
	if p.Format != "json" {
		model["skippedCnt"] = s.SkippedCnt
		model["failedCnt"] = s.FailedCnt
	}
	// EXISTING_CODE

	return labelAddresses(p, model, needed)
}

// CalcMap returns a map containing the calculated/derived fields for this type.
func (s *ReportCheck) CalcMap(p *ModelProps) map[string]any {
	_ = p // delint
	model := map[string]any{
		// EXISTING_CODE
		// EXISTING_CODE
	}

	// EXISTING_CODE
	if p.Format == "json" {
		if s.FailedCnt > 0 {
			model["failedCnt"] = s.FailedCnt
		}
		if s.SkippedCnt > 0 {
			model["skippedCnt"] = s.SkippedCnt
		}
		if len(s.MsgStrings) > 0 {
			model["msgStrings"] = s.MsgStrings
		}
	}
	// EXISTING_CODE

	return model
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *ReportCheck) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	s.Calcs = nil
	// EXISTING_CODE
	// EXISTING_CODE
}

// ReportCheckCalcs holds lazy-loaded calculated fields for ReportCheck
type ReportCheckCalcs struct {
	// EXISTING_CODE
	FailedCnt  uint64   `json:"failedCnt,omitempty"`
	SkippedCnt uint64   `json:"skippedCnt,omitempty"`
	MsgStrings []string `json:"msgStrings,omitempty"`
	// EXISTING_CODE
}

func (s *ReportCheck) EnsureCalcs(p *ModelProps, fieldFilter []string) error {
	_ = fieldFilter // delint
	if s.Calcs != nil {
		return nil
	}

	calcMap := s.CalcMap(p)
	if len(calcMap) == 0 {
		return nil
	}

	jsonBytes, err := json.Marshal(calcMap)
	if err != nil {
		return err
	}

	s.Calcs = &ReportCheckCalcs{}
	return json.Unmarshal(jsonBytes, s.Calcs)
}

// EXISTING_CODE
// EXISTING_CODE
