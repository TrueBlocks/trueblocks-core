// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// CheckSizes compares the file on disc to the file size suggested in the manifest
func (opts *ChunksOptions) CheckSizes(fileNames []string, blockNums []uint64, cacheManifest *manifest.Manifest, remoteManifest *manifest.Manifest, report *types.ReportCheck) error {

	theManifest := cacheManifest
	if len(cacheManifest.Chunks) > len(remoteManifest.Chunks) {
		theManifest = remoteManifest
	}

	indexSizeMap := make(map[base.FileRange]int64, len(theManifest.Chunks))
	bloomSizeMap := make(map[base.FileRange]int64, len(theManifest.Chunks))
	for _, r := range theManifest.Chunks {
		rng := base.RangeFromRangeString(r.Range)
		indexSizeMap[rng] = r.IndexSize
		bloomSizeMap[rng] = r.BloomSize
	}

	for _, fileName := range fileNames {
		report.VisitedCnt++
		report.CheckedCnt++
		indexFn := cache.ToIndexPath(fileName)
		rng := base.RangeFromFilename(indexFn)
		indexSize := file.FileSize(indexFn)
		if file.FileExists(indexFn) && indexSize != indexSizeMap[rng] {
			report.MsgStrings = append(report.MsgStrings, fmt.Sprintf("Size of index %s (%d) not as expected in manifest (%d)", rng, indexSize, indexSizeMap[rng]))
		} else {
			bloomFn := cache.ToBloomPath(fileName)
			bloomSize := file.FileSize(bloomFn)
			if bloomSize != bloomSizeMap[rng] {
				report.MsgStrings = append(report.MsgStrings, fmt.Sprintf("Size of bloom %s (%d) not as expected in manifest (%d)", rng, bloomSize, bloomSizeMap[rng]))
			} else {
				report.PassedCnt++
			}
		}
	}

	// TODO: We could check that the two manifests have the same files sizes here as well

	return nil
}
