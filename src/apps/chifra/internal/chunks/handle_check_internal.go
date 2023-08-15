// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/unchained"
)

// CheckInternal reads the header of each chunk on disc looking for the Magic number and
// the HeaderMagicHash for expected values.
func (opts *ChunksOptions) CheckInternal(fileNames []string, blockNums []uint64, report *simpleReportCheck) error {
	for testId, fileName := range fileNames {
		opts.checkIndexChunkInternal(testId, fileName, report)
		// opts.checkBloomInternal(testId, fileName, report)
	}
	return nil
}

func (opts *ChunksOptions) checkIndexChunkInternal(testId int, fileName string, report *simpleReportCheck) {
	report.VisitedCnt++
	report.CheckedCnt++
	header, err := index.ReadChunkHeader(fileName, true)
	if err != nil {
		if !strings.Contains(err.Error(), "no such file or directory") {
			report.MsgStrings = append(report.MsgStrings, fmt.Sprint(err))
		} else {
			// This is the case where the user did not download all the index chunks, only blooms
			report.PassedCnt++
		}

	} else {
		rng := base.RangeFromFilename(fileName)
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

// TODO: This work is incomplete
// func (opts *ChunksOptions) checkBloomInternal(testId int, fileName string, report *simpleReportCheck) {
// 	report.VisitedCnt++
// 	report.CheckedCnt++
// 	var bl bloom.ChunkBloom
// 	bPath := index.ToBloomPath(fileName)
// 	bl.ReadBloom(bPath)
// 	versioned, err := bl.ReadBloom Header()
// 	if err != nil {
// 		report.MsgStrings = append(report.MsgStrings, fmt.Sprint(err))
// 	} else if !versioned {
// 	} else {
// 		rng := base.RangeFromFilename(fileName)
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
