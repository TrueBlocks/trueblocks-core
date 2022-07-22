// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// TODO: Concurrent?
func (opts *ChunksOptions) CheckSequential(fnArray, cacheArray, remoteArray []string, report *types.ReportCheck) error {
	if err := opts.checkSequential("disc", fnArray, report); err != nil {
		return err
	}
	if err := opts.checkSequential("cache", cacheArray, report); err != nil {
		return err
	}
	if err := opts.checkSequential("contract", remoteArray, report); err != nil {
		return err
	}
	return nil
}

func (opts *ChunksOptions) checkSequential(which string, array []string, report *types.ReportCheck) error {
	// TODO: Technically, allow_missing means missing inside the file not in the filenames
	allow_missing := config.GetBlockScrapeSettings(opts.Globals.Chain).Allow_missing
	prev := cache.NotARange
	for _, item := range array {
		fR, _ := cache.RangeFromFilename(item)
		if prev != cache.NotARange {
			report.VisitedCnt++
			report.CheckedCnt++
			if prev != fR {
				if !fR.Follows(prev, !allow_missing) {
					report.MsgStrings = append(report.MsgStrings, fmt.Sprintf("In %s array, not sequental %s:%s", which, prev, fR))
				} else {
					report.PassedCnt++
				}
			} else {
				report.MsgStrings = append(report.MsgStrings, fmt.Sprintf("Duplicate at %s in %s array", fR, which))
			}
		}
		prev = fR
	}
	return nil
}
