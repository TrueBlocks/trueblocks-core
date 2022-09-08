// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"errors"
	"fmt"
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config/scrapeCfg"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/paths"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// HandleChunksCheck looks at three different arrays: index files on disc, manifest on disc,
// and manifest in the smart contract. It tries to check these three sources for
// cosnsistency. Smart contract rules, so it is checked more thoroughly.
func (opts *ChunksOptions) HandleChunksCheck(blockNums []uint64) error {
	// Checking only reports in JSON Mode
	opts.Globals.Format = "json"

	maxTestItems = 10
	filenameChan := make(chan paths.IndexFileInfo)

	var nRoutines int = 1
	go paths.WalkCacheFolder(opts.Globals.Chain, paths.Index_Bloom, filenameChan)

	fileNames := []string{}
	for result := range filenameChan {
		switch result.Type {
		case paths.Index_Bloom:
			skip := (opts.Globals.TestMode && len(fileNames) > maxTestItems) || !strings.HasSuffix(result.Path, ".bloom")
			if !skip {
				hit := false
				for _, block := range blockNums {
					h := result.Range.IntersectsB(block)
					hit = hit || h
					if hit {
						break
					}
				}
				if len(blockNums) == 0 || hit {
					fileNames = append(fileNames, result.Path)
				}
			}
		case paths.None:
			nRoutines--
			if nRoutines == 0 {
				close(filenameChan)
			}
		}
	}

	if len(fileNames) == 0 {
		msg := fmt.Sprint("No files found to check in", config.GetPathToIndex(opts.Globals.Chain))
		return errors.New(msg)
	}

	sort.Slice(fileNames, func(i, j int) bool {
		return fileNames[i] < fileNames[j]
	})

	cacheManifest, err := manifest.ReadManifest(opts.Globals.Chain, manifest.FromCache)
	if err != nil {
		return err
	}

	remoteManifest, err := manifest.ReadManifest(opts.Globals.Chain, manifest.FromContract)
	if err != nil {
		return err
	}

	// a string array of the actual files in the index
	fnArray := []string{}
	for _, fileName := range fileNames {
		rng := paths.RangeFromFilename(fileName)
		fnArray = append(fnArray, rng.String())
	}
	sort.Slice(fnArray, func(i, j int) bool {
		return fnArray[i] < fnArray[j]
	})

	// a string array of the ranges in the local manifest
	cacheArray := []string{}
	for _, chunk := range cacheManifest.Chunks {
		cacheArray = append(cacheArray, chunk.Range)
	}
	sort.Slice(cacheArray, func(i, j int) bool {
		return cacheArray[i] < cacheArray[j]
	})

	// a string array of the ranges from the remote manifest
	remoteArray := []string{}
	for _, chunk := range remoteManifest.Chunks {
		remoteArray = append(remoteArray, chunk.Range)
	}
	sort.Slice(remoteArray, func(i, j int) bool {
		return remoteArray[i] < remoteArray[j]
	})

	reports := []types.ReportCheck{}

	allowMissing := scrapeCfg.AllowMissing(opts.Globals.Chain)
	seq := types.ReportCheck{Reason: "Filenames sequential"}
	if err := opts.CheckSequential(fileNames, cacheArray, remoteArray, allowMissing, &seq); err != nil {
		return err
	}
	reports = append(reports, seq)

	intern := types.ReportCheck{Reason: "Internally consistent"}
	if err := opts.CheckInternal(fileNames, blockNums, &intern); err != nil {
		return err
	}
	reports = append(reports, intern)

	con := types.ReportCheck{Reason: "Consistent hashes"}
	if err := opts.CheckHashes(cacheManifest, remoteManifest, &con); err != nil {
		return err
	}
	reports = append(reports, con)

	sizes := types.ReportCheck{Reason: "Check file sizes"}
	if err := opts.CheckSizes(fileNames, blockNums, cacheManifest, remoteManifest, &sizes); err != nil {
		return err
	}
	reports = append(reports, sizes)

	// compare remote manifest to cached manifest
	r2c := types.ReportCheck{Reason: "Remote Manifest to Cached Manifest"}
	if err := opts.CheckManifest(remoteArray, cacheArray, &r2c); err != nil {
		return err
	}
	reports = append(reports, r2c)

	// compare with çached manifest with files on disc
	d2c := types.ReportCheck{Reason: "Disc Files to Cached Manifest"}
	if err := opts.CheckManifest(fnArray, cacheArray, &d2c); err != nil {
		return err
	}
	reports = append(reports, d2c)

	// compare with remote manifest with files on disc
	d2r := types.ReportCheck{Reason: "Disc Files to Remote Manifest"}
	if err := opts.CheckManifest(fnArray, remoteArray, &d2r); err != nil {
		return err
	}
	reports = append(reports, d2r)

	for i := 0; i < len(reports); i++ {
		reports[i].FailedCnt = reports[i].CheckedCnt - reports[i].PassedCnt
		if reports[i].FailedCnt == 0 {
			reports[i].PassedCnt = 0
			reports[i].VisitedCnt = 0
			reports[i].Result = "passed"
		} else {
			reports[i].SkippedCnt = reports[i].VisitedCnt - reports[i].CheckedCnt
		}
	}
	return globals.RenderSlice(&opts.Globals, reports)
}
