// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"fmt"
	"strconv"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
)

// CheckStaging checks the staging file which should be names first-second.txt
//  1. Makes sure that finalized + 1 == first
//  2. Makes sure the first < last
//  3. Makes sure that the first block inside is == first if allow_missing == false, > otherwise
//  4. Makes sure that the last block inside is == last if allow_missing == false, < otherwise
func (opts *ChunksOptions) CheckStaging(lastBlock uint64, allow_missing bool, report *simpleReportCheck) error {
	chain := opts.Globals.Chain
	stagePath := index.ToStagingPath(config.GetPathToIndex(chain) + "staging")
	stageFn, _ := file.LatestFileInFolder(stagePath)
	if !file.FileExists(stageFn) {
		return nil
	}

	fileRange := base.RangeFromFilename(stageFn)
	meta, err := opts.Conn.GetMetaData(chain, opts.Globals.TestMode)
	if err != nil {
		return err
	}

	const asciiAppearanceSize = 59
	nApps := int(file.FileSize(stageFn) / asciiAppearanceSize)

	// It's okay for the file to be empty
	if nApps == 0 {
		return nil
	}

	report.VisitedCnt++

	//  1. Makes sure that finalized + 1 == first (adjust for allow_missing)
	report.CheckedCnt++
	if meta.Finalized+1 == fileRange.First || (allow_missing && meta.Finalized+1 <= fileRange.First) {
		report.PassedCnt++
	} else {
		report.MsgStrings = append(report.MsgStrings, fmt.Sprintf("Finalized block (%d) + 1 != first block (%d)", meta.Finalized, fileRange.First))
	}

	//  2. Makes sure the first < last
	report.CheckedCnt++
	if fileRange.First <= fileRange.Last {
		report.PassedCnt++
	} else {
		report.MsgStrings = append(report.MsgStrings, fmt.Sprintf("First block (%d) > last block (%d)", fileRange.First, fileRange.Last))
	}

	appearances := file.AsciiFileToLines(stageFn)
	if len(appearances) == 0 {
		// should never happen, but bail if it does
		return nil
	}

	//  3. Makes sure that the first block inside is == first if allow_missing == false, > otherwise
	report.CheckedCnt++
	first := mustParseUint(strings.Split(appearances[0], "\t")[1])
	if first == fileRange.First || (allow_missing && first <= fileRange.First) {
		report.PassedCnt++
	} else {
		report.MsgStrings = append(report.MsgStrings, fmt.Sprintf("First block inside (%d) != first block (%d)", first, fileRange.First))
	}

	//  4. Makes sure that the last block inside is == last if allow_missing == false, < otherwise
	report.CheckedCnt++
	last := mustParseUint(strings.Split(appearances[len(appearances)-1], "\t")[1])
	if last == fileRange.Last || (allow_missing && last <= fileRange.Last) {
		report.PassedCnt++
	} else {
		report.MsgStrings = append(report.MsgStrings, fmt.Sprintf("Last block inside (%d) != last block (%d)", last, fileRange.Last))
	}

	return nil
}

func mustParseUint(input string) (result uint64) {
	result, _ = strconv.ParseUint(strings.TrimLeft(input, "0"), 0, 64)
	return
}
