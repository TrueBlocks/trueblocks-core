// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package types

// EXISTING_CODE
import "io"

// EXISTING_CODE

type RawReportCheck struct {
	Reason     string `json:"reason"`
	CheckedCnt string `json:"checkedCnt"`
	FailedCnt  string `json:"failedCnt"`
	MsgStrings string `json:"msgStrings"`
	PassedCnt  string `json:"passedCnt"`
	Result     string `json:"result"`
	SkippedCnt string `json:"skippedCnt"`
	VisitedCnt string `json:"visitedCnt"`
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleReportCheck struct {
	Reason     string          `json:"reason"`
	CheckedCnt uint32          `json:"checkedCnt"`
	FailedCnt  uint32          `json:"failedCnt"`
	MsgStrings []string        `json:"msgStrings"`
	PassedCnt  uint32          `json:"passedCnt"`
	Result     string          `json:"result"`
	SkippedCnt uint32          `json:"skippedCnt"`
	VisitedCnt uint32          `json:"visitedCnt"`
	raw        *RawReportCheck `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimpleReportCheck) Raw() *RawReportCheck {
	return s.raw
}

func (s *SimpleReportCheck) SetRaw(raw *RawReportCheck) {
	s.raw = raw
}

func (s *SimpleReportCheck) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]any{
		"reason":     s.Reason,
		"result":     s.Result,
		"checkedCnt": s.CheckedCnt,
	}
	order = []string{
		"reason",
		"result",
		"checkedCnt",
		"visitedCnt",
		"passedCnt",
		"skippedCnt",
		"failedCnt",
		"msgStrings",
	}
	if s.VisitedCnt > 0 {
		model["visitedCnt"] = s.VisitedCnt
	}
	if s.PassedCnt > 0 {
		model["passedCnt"] = s.PassedCnt
	}
	if s.SkippedCnt > 0 {
		model["skippedCnt"] = s.SkippedCnt
	}
	if s.FailedCnt > 0 {
		model["failedCnt"] = s.FailedCnt
	}
	if len(s.MsgStrings) > 0 {
		model["msgStrings"] = s.MsgStrings
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *SimpleReportCheck) WriteTo(w io.Writer) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

func (s *SimpleReportCheck) ReadFrom(r io.Reader) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

// EXISTING_CODE
// EXISTING_CODE
