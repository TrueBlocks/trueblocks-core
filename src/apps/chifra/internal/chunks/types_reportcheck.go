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

func (s *simpleReportCheck) Model(verbose bool, format string, extraOptions map[string]any) types.Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]any{
		"result":     s.Result,
		"checkedCnt": s.CheckedCnt,
		"visitedCnt": s.VisitedCnt,
		"passedCnt":  s.PassedCnt,
		"reason":     s.Reason,
	}
	order = []string{
		"result",
		"checkedCnt",
		"visitedCnt",
		"passedCnt",
	}
	if s.FailedCnt > 0 {
		model["failedCnt"] = s.FailedCnt
		order = append(order, "failedCnt")
	}
	if s.SkippedCnt > 0 {
		model["skippedCnt"] = s.SkippedCnt
		order = append(order, "skippedCnt")
	}
	order = append(order, "reason")
	if format == "json" {
		if len(s.MsgStrings) > 0 {
			model["msgStrings"] = s.MsgStrings
			order = append(order, "msgStrings")
		}
	}
	// EXISTING_CODE

	return types.Model{
		Data:  model,
		Order: order,
	}
}

// EXISTING_CODE
// EXISTING_CODE
