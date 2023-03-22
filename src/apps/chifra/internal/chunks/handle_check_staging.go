// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// CheckStaging looks the stage and makes sure the blocks in the file agree with the file's name. It also
// checks that the first block is one plus the last block in the finalized index and that the file contains
// sequential blocks if allow_missing is not on.
func (opts *ChunksOptions) CheckStaging(lastBlock uint64, allow_missing bool, report *types.ReportCheck) error {
	stagePath := cache.ToStagingPath(config.GetPathToIndex(opts.Globals.Chain) + "staging")
	stageFn, _ := file.LatestFileInFolder(stagePath)
	if !file.FileExists(stageFn) {
		return nil
	}
	report.VisitedCnt++
	report.CheckedCnt++
	// report.PassedCnt++
	report.FailedCnt++
	rng := base.RangeFromFilename(stageFn)
	r := rng.First < rng.Last
	report.Result = rng.String() + fmt.Sprintf(" okay: %t", r)
	return nil
}

// func (opts *ChunksOptions) checkHashes(which string, man *manifest.Manifest, report *types.ReportCheck) error {
// 	for _, chunk := range man.Chunks {
// 		report.VisitedCnt++
// 		report.CheckedCnt++
// 		if len(chunk.BloomHash) != 46 {
// 			msg := fmt.Sprintf("%s: Invalid BloomHash (%s) in %s", chunk.Range, chunk.BloomHash, which)
// 			if len(chunk.BloomHash) == 0 {
// 				msg = fmt.Sprintf("%s: Empty BloomHash in %s", chunk.Range, which)
// 			}
// 			report.MsgStrings = append(report.MsgStrings, msg)
// 		} else {
// 			report.PassedCnt++
// 		}
// 		report.VisitedCnt++
// 		report.CheckedCnt++
// 		if len(chunk.IndexHash) != 46 {
// 			msg := fmt.Sprintf("%s: Invalid IndexHash (%s) in %s", chunk.Range, chunk.IndexHash, which)
// 			if len(chunk.IndexHash) == 0 {
// 				msg = fmt.Sprintf("%s: Empty IndexHash in %s", chunk.Range, which)
// 			}
// 			report.MsgStrings = append(report.MsgStrings, msg)
// 		} else {
// 			report.PassedCnt++
// 		}
// 	}
// 	return nil
// }
