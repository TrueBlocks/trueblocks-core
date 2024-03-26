// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package chunksPkg

// EXISTING_CODE
import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"

// EXISTING_CODE

type simpleReportCheck struct {
	CheckedCnt uint32   `json:"checkedCnt"`
	FailedCnt  uint32   `json:"failedCnt"`
	MsgStrings []string `json:"msgStrings"`
	PassedCnt  uint32   `json:"passedCnt"`
	Reason     string   `json:"reason"`
	Result     string   `json:"result"`
	SkippedCnt uint32   `json:"skippedCnt"`
	VisitedCnt uint32   `json:"visitedCnt"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *simpleReportCheck) Raw() *types.RawModeler {
	return nil
}

func (s *simpleReportCheck) Model(chain, format string, verbose bool, extraOptions map[string]any) types.Model {
	var model = map[string]interface{}{}
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

	return types.Model{
		Data:  model,
		Order: order,
	}
}

// EXISTING_CODE
// EXISTING_CODE
