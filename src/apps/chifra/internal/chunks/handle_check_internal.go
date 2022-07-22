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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/unchained"
)

// TODO: Concurrent?
func (opts *ChunksOptions) CheckInternal(fileNames []string, blockNums []uint64, report *types.ReportCheck) error {
	for testId, fileName := range fileNames {
		report.VisitedCnt++
		report.CheckedCnt++
		header, err := index.ReadChunkHeader(opts.Globals.Chain, fileName)
		if err != nil {
			report.MsgStrings = append(report.MsgStrings, fmt.Sprint(err))

		} else {
			rng, _ := cache.RangeFromFilename(fileName)
			if !opts.Globals.TestMode {
				testId = 0
			}

			if header.Magic != file.MagicNumber || (testId == 1) {
				msg := fmt.Sprintf("%s: Magic number expected (0x%x) got (0x%x)", rng, header.Magic, file.MagicNumber)
				report.MsgStrings = append(report.MsgStrings, msg)

			} else if header.Hash.Hex() != unchained.ZeroMagicHash && header.Hash.Hex() != unchained.HeaderMagicHash || (testId == 2) {
				// TODO: BOGUS - MIGRATION CHECKING HASHES
				msg := fmt.Sprintf("%s: Header hash expected (%s) got (%s)", rng, header.Hash.Hex(), unchained.HeaderMagicHash)
				report.MsgStrings = append(report.MsgStrings, msg)

			} else if rng.First > 3000000 && header.AppearanceCount > 2005000 || testId == 3 {
				// TODO: BOGUS - MIGRATION INVALID TEST
				msg := fmt.Sprintf("%s: Too many addresses? (%d)", rng, header.AppearanceCount)
				report.MsgStrings = append(report.MsgStrings, msg)

			} else {
				report.PassedCnt++
			}
		}
	}
	return nil
}
