// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

type CompareState struct {
	arrayA   []string
	arrayB   []string
	msg      string
	fail     int
	testMode bool
	details  bool
	// msgB     string
	// failB    int
}

// CheckManifest takes two arrays (either onDisc vs. LocalManifest, onDisc vs. RemoteManifest, or LocalManifest
// vs. RemoteManifest) and compares them for equality. If everything is up to date, all three arrays should
// be identical. Only the block ranges are in the arrays.
func (opts *ChunksOptions) CheckManifest(arrayA, arrayB []string, report *types.SimpleReportCheck) error {
	comp := CompareState{
		testMode: opts.Globals.TestMode,
		details:  opts.Globals.Verbose,
		msg:      "%s: The chunk is in the %s array but not the %s array%s",
		fail:     3,
		arrayA:   arrayA,
		arrayB:   arrayB,
	}

	if err := comp.checkArrays(report); err != nil {
		return err
	}

	comp.arrayA, comp.arrayB = comp.arrayB, comp.arrayA
	comp.msg = "%s: The chunk is in the %s array but not the %s array%s"
	comp.fail = 4

	return comp.checkArrays(report)
}

// TODO: Can this be made concurrent?
func (comp *CompareState) checkArrays(report *types.SimpleReportCheck) error {
	marker := ""
	if comp.testMode {
		marker = " (testing)"
	}

	if !comp.testMode && len(comp.arrayA) != len(comp.arrayB) {
		alreadyReported := false
		for _, e := range report.MsgStrings {
			if strings.Contains(e, "array lengths are different") {
				alreadyReported = true
			}
		}
		if !alreadyReported {
			parts := strings.Split(report.Reason, " to ")
			msg := fmt.Sprintf("array lengths are different: %s(%d) %s(%d)", parts[0], len(comp.arrayA), parts[1], len(comp.arrayB))
			report.MsgStrings = append(report.MsgStrings, msg)
		}
	}

	theMap := make(map[string]bool, len(comp.arrayA))
	for _, item := range comp.arrayA {
		theMap[item] = true
	}

	parts := strings.Split(report.Reason, " to ")
	for testId, item := range comp.arrayB {
		if testId > len(comp.arrayA)-1 {
			continue
		}
		if !comp.testMode {
			testId = -1
		}
		report.VisitedCnt++
		report.CheckedCnt++
		if !theMap[item] || testId == comp.fail {
			if len(report.MsgStrings) < 4 || comp.details {
				parts[0] = strings.ToLower(parts[0])
				parts[1] = strings.ToLower(parts[1])
				msg := fmt.Sprintf(comp.msg, item, parts[0], parts[1], marker)
				report.MsgStrings = append(report.MsgStrings, msg)
			}
		} else {
			report.PassedCnt++
		}
	}

	return nil
}
