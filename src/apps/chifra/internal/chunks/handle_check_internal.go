// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// TODO: Concurrent?
func (opts *ChunksOptions) CheckInternal(fileNames []string, blockNums []uint64, report *types.CheckReport) error {
	// allow_missing := config.ReadBlockScrape(opts.Globals.Chain).Settings.Allow_missing
	for testId, fileName := range fileNames {
		report.VisitedCnt++
		report.CheckedCnt++
		header, err := index.ReadHeaderFromFilename(fileName)
		if err != nil {
			report.ErrorStrs = append(report.ErrorStrs, fmt.Sprint(err))

		} else {
			rng, _ := cache.RangeFromFilename(fileName)
			if !opts.Globals.TestMode {
				testId = 0
			}

			if header.Magic != file.MagicNumber || (testId == 1) {
				msg := fmt.Sprintf("%s: Magic number expected (0x%x) got (0x%x)", rng, header.Magic, file.MagicNumber)
				report.ErrorStrs = append(report.ErrorStrs, msg)

			} else if header.Hash.Hex() != index.ZeroMagicHash && header.Hash.Hex() != index.HeaderMagicHash || (testId == 2) {
				msg := fmt.Sprintf("%s: Header hash expected (%s) got (%s)", rng, header.Hash.Hex(), index.HeaderMagicHash)
				report.ErrorStrs = append(report.ErrorStrs, msg)

			} else if rng.First > 3000000 && header.AppearanceCount > 2005000 || testId == 3 {
				// TODO: Multi-chain specific -- this is a bogus test
				msg := fmt.Sprintf("%s: Too many addresses? (%d)", rng, header.AppearanceCount)
				report.ErrorStrs = append(report.ErrorStrs, msg)

			} else {
				report.PassedCnt++
				// fmt.Println(header)
			}
		}
	}

	report.FailedCnt = report.CheckedCnt - report.PassedCnt
	report.SkippedCnt = report.VisitedCnt - report.CheckedCnt
	return nil
}
