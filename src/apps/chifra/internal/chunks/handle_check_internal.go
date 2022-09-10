// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/paths"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/unchained"
)

// CheckInternal reads the header of each chunk on disc looking for the Magic number and
// the HeaderMagicHash for expected values.
func (opts *ChunksOptions) CheckInternal(fileNames []string, blockNums []uint64, report *types.ReportCheck) error {
	for testId, fileName := range fileNames {
		opts.checkIndexChunkInternal(testId, fileName, report)
		// opts.checkBloomInternal(testId, fileName, report)
	}
	return nil
}

func (opts *ChunksOptions) checkIndexChunkInternal(testId int, fileName string, report *types.ReportCheck) {
	report.VisitedCnt++
	report.CheckedCnt++
	header, err := index.ReadChunkHeader(opts.Globals.Chain, fileName, true)
	if err != nil {
		report.MsgStrings = append(report.MsgStrings, fmt.Sprint(err))

	} else {
		rng := paths.RangeFromFilename(fileName)
		if !opts.Globals.TestMode {
			testId = 0
		}

		if header.Magic != file.MagicNumber || (testId == 1) {
			msg := fmt.Sprintf("%s: Magic number expected (0x%x) got (0x%x)", rng, header.Magic, file.MagicNumber)
			report.MsgStrings = append(report.MsgStrings, msg)

		} else if header.Hash.Hex() != unchained.HeaderMagicHash || (testId == 2) {
			msg := fmt.Sprintf("%s: Header hash expected (%s) got (%s)", rng, header.Hash.Hex(), unchained.HeaderMagicHash)
			report.MsgStrings = append(report.MsgStrings, msg)

		} else {
			report.PassedCnt++
		}
	}
}

// func (opts *ChunksOptions) checkBloomInternal(testId int, fileName string, report *types.ReportCheck) {
// 	report.VisitedCnt++
// 	report.CheckedCnt++
// 	var bl bloom.ChunkBloom
// 	bPath := paths.ToBloomPath(fileName)
// 	bl.ReadBloom(bPath)
// 	versioned, err := bl.ReadBloomHeader()
// 	if err != nil {
// 		report.MsgStrings = append(report.MsgStrings, fmt.Sprint(err))
// 	} else if !versioned {
// 	} else {
// 		rng := paths.RangeFromFilename(fileName)
// 		if !opts.Globals.TestMode {
// 			testId = 0
// 		}

// 		if bl.Header.Magic != file.SmallMagicNumber {
// 			msg := fmt.Sprintf("%s: Magic number expected (0x%x) got (0x%x)", rng, bl.Header.Magic, file.SmallMagicNumber)
// 			report.MsgStrings = append(report.MsgStrings, msg)
// 		} else {
// 			report.PassedCnt++
// 		}
// 	}
// }
