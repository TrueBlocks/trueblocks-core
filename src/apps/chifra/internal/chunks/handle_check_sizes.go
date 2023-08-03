// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
)

// CheckSizes compares the file on disc to the file size suggested in the manifest
func (opts *ChunksOptions) CheckSizes(fileNames []string, blockNums []uint64, cacheManifest *manifest.Manifest, remoteManifest *manifest.Manifest, report *simpleReportCheck) error {

	// Figure out which manifest we're going to check against
	theManifest := cacheManifest
	if len(cacheManifest.Chunks) > len(remoteManifest.Chunks) {
		theManifest = remoteManifest
	}

	// Store the sizes of the index and bloom files in the manifest for easy access
	idxSizeInMan := make(map[base.FileRange]int64, len(theManifest.Chunks))
	bloomSizeInMan := make(map[base.FileRange]int64, len(theManifest.Chunks))
	maxInManifest := base.FileRange{}
	for _, r := range theManifest.Chunks {
		rng := base.RangeFromRangeString(r.Range)
		idxSizeInMan[rng] = r.IndexSize
		bloomSizeInMan[rng] = r.BloomSize
		if rng.LaterThan(maxInManifest) {
			maxInManifest = rng
		}
	}

	// We will check both the index and the bloom even though `--check` is only available for index
	for _, fileName := range fileNames {
		indexFn := index.ToIndexPath(fileName)
		rng := base.RangeFromFilename(indexFn)
		if !rng.LaterThan(maxInManifest) {
			okay := true // the test passes only if both pass unless there's only one
			if file.FileExists(indexFn) {
				indexSize := file.FileSize(indexFn)
				if indexSize != idxSizeInMan[rng] {
					report.MsgStrings = append(report.MsgStrings, fmt.Sprintf("Size of index %s (%d) not as expected in manifest (%d)", rng, indexSize, idxSizeInMan[rng]))
					okay = false
				}
			}

			if okay {
				bloomFn := index.ToBloomPath(fileName)
				if file.FileExists(bloomFn) {
					bloomSize := file.FileSize(bloomFn)
					if bloomSize != bloomSizeInMan[rng] {
						report.MsgStrings = append(report.MsgStrings, fmt.Sprintf("Size of bloom %s (%d) not as expected in manifest (%d)", rng, bloomSize, bloomSizeInMan[rng]))
						okay = false
					}
				}
			}
			if okay {
				report.PassedCnt++
			}
			report.CheckedCnt++
		}
		report.VisitedCnt++
	}

	// TODO: We could check that the two manifests have the same files sizes here as well

	return nil
}
