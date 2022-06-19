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
func (opts *ChunksOptions) CheckSequential(fileNames []string, blockNums []uint64, report *types.CheckReport) error {
	// TODO: Technically, allow_missing means missing inside the file not in the filenames
	allow_missing := config.ReadBlockScrape(opts.Globals.Chain).Settings.Allow_missing

	// We don't check the first one since there's nothing before it
	report.VisitedCnt++
	report.CheckedCnt++
	report.PassedCnt++

	prev := cache.NotARange
	for _, fileName := range fileNames {
		fR, _ := cache.RangeFromFilename(fileName)
		if prev != cache.NotARange && prev != fR {
			report.VisitedCnt++
			report.CheckedCnt++
			if !fR.Follows(prev, !allow_missing) {
				report.FailedCnt++
				report.ErrorStrs = append(report.ErrorStrs, fmt.Sprintf("%s does not sequentially follow %s", fR, prev))
				report.FailedCnt++
			} else {
				report.PassedCnt++
			}
		}
		prev = fR
	}
	report.FailedCnt = report.CheckedCnt - report.PassedCnt
	report.SkippedCnt = report.VisitedCnt - report.CheckedCnt
	return nil
}
