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
	CheckedCnt uint64   `json:"checkedCnt"`
	FailedCnt  uint64   `json:"failedCnt"`
	MsgStrings []string `json:"msgStrings"`
	PassedCnt  uint64   `json:"passedCnt"`
	Reason     string   `json:"reason"`
	Result     string   `json:"result"`
	SkippedCnt uint64   `json:"skippedCnt"`
	VisitedCnt uint64   `json:"visitedCnt"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s ReportCheck) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *ReportCheck) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	var model = map[string]any{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]any{
		"result":     s.Result,
		"checkedCnt": s.CheckedCnt,
		"visitedCnt": s.VisitedCnt,
		"passedCnt":  s.PassedCnt,
	}
	order = []string{
		"result",
		"checkedCnt",
		"visitedCnt",
		"passedCnt",
	}

	if format == "json" {
		if s.FailedCnt > 0 {
			model["failedCnt"] = s.FailedCnt
		}
		if s.SkippedCnt > 0 {
			model["skippedCnt"] = s.SkippedCnt
		}
		model["reason"] = s.Reason
		if len(s.MsgStrings) > 0 {
			model["msgStrings"] = s.MsgStrings
		}
	} else {
		model["skippedCnt"] = s.SkippedCnt
		model["failedCnt"] = s.FailedCnt
		model["reason"] = s.Reason
		order = append(order, []string{"skippedCnt", "failedCnt", "reason"}...)
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *ReportCheck) FinishUnmarshal(fileVersion uint64) {
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
