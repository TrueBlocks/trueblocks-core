// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"fmt"
	"sort"
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

func (opts *ChunksOptions) CheckManifest(arrayA, arrayB []string, report *types.CheckReport) error {
	sort.Slice(arrayA, func(i, j int) bool {
		return arrayA[i] < arrayA[j]
	})
	sort.Slice(arrayB, func(i, j int) bool {
		return arrayB[i] < arrayB[j]
	})

	comp := CompareState{
		testMode: opts.Globals.TestMode,
		details:  opts.Details,
		msg:      "%s: The chunk is in the first but not the second array%s",
		fail:     3,
		arrayA:   arrayA,
		arrayB:   arrayB,
	}

	if err := comp.checkArrays(report); err != nil {
		return err
	}

	comp.arrayA, comp.arrayB = comp.arrayB, comp.arrayA
	comp.msg = "%s: The chunk is in the second but not the first array%s"
	comp.fail = 4

	return comp.checkArrays(report)
}

// TODO: Concurrent?
func (comp *CompareState) checkArrays(report *types.CheckReport) error {
	marker := ""
	if comp.testMode {
		marker = " (testing)"
	}

	if !comp.testMode && len(comp.arrayA) != len(comp.arrayB) {
		alreadyReported := false
		for _, e := range report.ErrorStrs {
			if strings.Contains(e, "array lengths are different") {
				alreadyReported = true
			}
		}
		if !alreadyReported {
			msg := fmt.Sprintf("array lengths are different: arrayA(%d) arrayB(%d)", len(comp.arrayA), len(comp.arrayB))
			report.ErrorStrs = append(report.ErrorStrs, msg)
		}
	}

	theMap := make(map[string]bool, len(comp.arrayA))
	for _, item := range comp.arrayA {
		theMap[item] = true
	}

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
			if len(report.ErrorStrs) < 4 || comp.details {
				msg := fmt.Sprintf(comp.msg, item, marker)
				report.ErrorStrs = append(report.ErrorStrs, msg)
			}
		} else {
			report.PassedCnt++
		}
	}

	report.FailedCnt = report.CheckedCnt - report.PassedCnt
	report.SkippedCnt = report.VisitedCnt - report.CheckedCnt
	return nil
}
