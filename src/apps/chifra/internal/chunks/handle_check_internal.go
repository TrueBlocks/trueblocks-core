// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"fmt"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
)

// CheckInternal reads the header of each chunk on disc looking for the Magic number and
// the hash of the SpecVersion for expected values.
func (opts *ChunksOptions) CheckInternal(fileNames []string, blockNums []uint64, report *simpleReportCheck) error {
	for _, fileName := range fileNames {
		opts.checkIndexChunkInternal(fileName, report)
		// opts.checkBloomInternal(testId, fileName, report)
	}
	return nil
}

func (opts *ChunksOptions) checkIndexChunkInternal(fileName string, report *simpleReportCheck) {
	report.VisitedCnt++
	report.CheckedCnt++

	var err error
	var idx index.Index

	fileName = index.ToIndexPath(fileName)
	idx.File, err = os.OpenFile(fileName, os.O_RDONLY, 0)
	if err != nil {
		return
	}
	defer idx.File.Close()

	if _, err := idx.ReadHeader(config.HeaderTag(), false /* unused */); err != nil {
		if strings.Contains(err.Error(), "no such file or directory") {
			// This is the case where the user did not download all the index chunks, only blooms
			report.PassedCnt++
		} else {
			report.MsgStrings = append(report.MsgStrings, fmt.Sprintf("%s: %s", err, fileName))
		}

	} else {
		report.PassedCnt++
	}
}
