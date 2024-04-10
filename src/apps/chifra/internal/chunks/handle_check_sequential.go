// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// CheckSequential checks that the range of blocks in each of three arrays (onDisc,
// LocalManifest, RemoteManifest) "cover" the range without gaps. (There may be a bug
// for allow_missing chains where gaps are allowed.) It also makes sure than snap-to-grids
// happen where they should and that non-snaps have at least appsPerChunks records and
// snaps have exactly appsPerChunks records or less.
func (opts *ChunksOptions) CheckSequential(fnArray, cacheArray, remoteArray []string, allowMissing bool, report *types.SimpleReportCheck) error {
	if err := opts.checkSequential("disc", fnArray, allowMissing, report); err != nil {
		return err
	}

	if err := opts.checkSequential("cache", cacheArray, allowMissing, report); err != nil {
		return err
	}

	if err := opts.checkSequential("contract", remoteArray, allowMissing, report); err != nil {
		return err
	}

	return nil
}

// TODO: Can this be made concurrent?
func (opts *ChunksOptions) checkSequential(which string, array []string, allowMissing bool, report *types.SimpleReportCheck) error {
	prev := base.NotARange
	for _, item := range array {
		var fR base.FileRange
		var err error
		if fR, err = base.RangeFromFilenameE(item); err != nil {
			return err
		}

		w := utils.MakeFirstUpperCase(which)
		if prev != base.NotARange {
			report.VisitedCnt++
			report.CheckedCnt++
			if prev != fR {
				if !prev.Preceeds(fR, !allowMissing) {
					// try to figure out why
					if prev.Intersects(fR) {
						report.MsgStrings = append(report.MsgStrings, fmt.Sprintf("%s: file ranges intersect %s:%s", w, prev, fR))
					} else if prev.Follows(fR, !allowMissing) {
						report.MsgStrings = append(report.MsgStrings, fmt.Sprintf("%s: previous range is after current %s:%s", w, prev, fR))
					} else if prev.Preceeds(fR, false) {
						report.MsgStrings = append(report.MsgStrings, fmt.Sprintf("%s: gap in sequence %d to %d skips %d", w, prev.Last, fR.First, (fR.First-prev.Last-1)))
					} else {
						report.MsgStrings = append(report.MsgStrings, fmt.Sprintf("%s: files not sequental %s:%s", w, prev, fR))
					}
				} else {
					report.PassedCnt++
				}
			} else {
				report.MsgStrings = append(report.MsgStrings, fmt.Sprintf("%s: duplicate at %s", w, fR))
			}
		}
		prev = fR
	}
	return nil
}
