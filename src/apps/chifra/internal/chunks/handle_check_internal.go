// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"errors"
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// CheckInternal reads the header of each chunk on disc looking for the Magic number and
// the hash of the spec version for expected values.
func (opts *ChunksOptions) CheckInternal(fileNames []string, blockNums []uint64, report *types.ReportCheck) error {
	for _, fileName := range fileNames {
		opts.checkIndexChunkInternal(fileName, false /* check version */, report)
		// opts.checkBloomInternal(testId, fileName, report)
	}
	return nil
}

func (opts *ChunksOptions) checkIndexChunkInternal(fileName string, checkVersion bool, report *types.ReportCheck) {
	report.VisitedCnt++
	report.CheckedCnt++

	fileName = index.ToIndexPath(fileName)
	indexChunk, err := index.OpenIndex(fileName, true /* check */)
	if err != nil {
		if strings.Contains(err.Error(), "no such file or directory") {
			// This is the case where the user did not download all the index chunks, only blooms
			report.PassedCnt++
		} else if !checkVersion && errors.Is(err, index.ErrIncorrectHash) {
			report.PassedCnt++
		} else {
			report.MsgStrings = append(report.MsgStrings, fmt.Sprintf("%s: %s", err, fileName))
		}
	} else {
		report.PassedCnt++
		if !checkVersion {
			opts.checkSnaps(fileName, &indexChunk, report)
		}
	}

	indexChunk.Close()
}

func (opts *ChunksOptions) checkSnaps(fileName string, indexChunk *index.Index, report *types.ReportCheck) {
	report.VisitedCnt++
	report.CheckedCnt++

	// we will check the manifest since it's the gold standard
	isSnap := func(fR base.FileRange, snapMarker, firstSnap uint64) bool {
		return fR.Last >= firstSnap && fR.Last%snapMarker == 0
	}

	chain := opts.Globals.Chain
	firstSnap := config.GetScrape(chain).FirstSnap
	snapMarker := config.GetScrape(chain).SnapToGrid
	appsPer := uint32(config.GetScrape(chain).AppsPerChunk)
	if fR, err := base.RangeFromFilenameE(fileName); err != nil {
		report.MsgStrings = append(report.MsgStrings, fmt.Sprintf("%s: %s", err, fileName))
	} else {
		if isSnap(fR, snapMarker, firstSnap) {
			if fR.Last < firstSnap {
				// Is there a snap_to_grid after first_snap everywhere it's supposed to be?
				report.MsgStrings = append(report.MsgStrings, fmt.Sprintf("checkSnap: snap too early %s firstSnap=%d", fR, firstSnap))
			} else if indexChunk.Header.AppearanceCount >= appsPer {
				// For snapped chunks, nApps < apps_per_chunk.
				report.MsgStrings = append(report.MsgStrings, fmt.Sprintf("contract: too many apps at %s appsPer=%d", fR, appsPer))
			} else {
				report.PassedCnt++
			}
		} else {
			if indexChunk.Header.AppearanceCount < appsPer && fR.Last > 0 {
				// For non-snapped chunks, nApps ≥ apps_per_chunk.
				report.MsgStrings = append(report.MsgStrings, fmt.Sprintf("checkSnap: too few appearances at %s appsPer=%d", fR, appsPer))
			} else {
				report.PassedCnt++
			}
		}
	}
}
