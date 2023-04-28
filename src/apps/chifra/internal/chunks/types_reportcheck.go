// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package chunksPkg

// EXISTING_CODE
import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"

// EXISTING_CODE

type simpleReportCheck struct {
	Reason     string   `json:"reason"`
	CheckedCnt uint32   `json:"checkedCnt"`
	FailedCnt  uint32   `json:"failedCnt"`
	MsgStrings []string `json:"msgStrings"`
	PassedCnt  uint32   `json:"passedCnt"`
	Result     string   `json:"result"`
	SkippedCnt uint32   `json:"skippedCnt"`
	VisitedCnt uint32   `json:"visitedCnt"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *simpleReportCheck) Raw() *types.RawModeler {
	return nil
}

func (s *simpleReportCheck) Model(verbose bool, format string, extraOptions map[string]any) types.Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]any{
		"reason":     s.Reason,
		"result":     s.Result,
		"checkedCnt": s.CheckedCnt,
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
	// EXISTING_CODE

	return types.Model{
		Data:  model,
		Order: order,
	}
}

// EXISTING_CODE
// EXISTING_CODE
