// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"errors"
	"fmt"
	"path/filepath"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/history"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ranges"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

// HandleCheck looks at three different arrays: index files on disc, manifest on disc,
// and manifest in the smart contract. It tries to check these three sources for
// cosnsistency. Smart contract rules, so it is checked more thoroughly.
func (opts *ChunksOptions) HandleCheck(rCtx *output.RenderCtx, blockNums []base.Blknum) error {
	err, _ := opts.check(rCtx, blockNums, false /* silent */)
	return err
}

// check provides internal checks against the index used from the command line and internally before pinning
func (opts *ChunksOptions) check(rCtx *output.RenderCtx, blockNums []base.Blknum, silent bool) (error, bool) {
	chain := opts.Globals.Chain

	maxTestItems := 10
	filenameChan := make(chan walk.CacheFileInfo)

	var nRoutines = 1
	go walk.WalkCacheFolder(rCtx.Ctx, chain, walk.Index_Bloom, nil, filenameChan)

	fileNames := []string{}
	for result := range filenameChan {
		switch result.Type {
		case walk.Index_Bloom:
			skip := (opts.Globals.TestMode && len(fileNames) > maxTestItems) || !walk.IsCacheType(result.Path, walk.Index_Bloom, true /* checkExt */)
			if !skip {
				hit := false
				for _, block := range blockNums {
					h := result.FileRange.IntersectsB(block)
					hit = hit || h
					if hit {
						break
					}
				}
				if len(blockNums) == 0 || hit {
					fileNames = append(fileNames, result.Path)
				}
			}
		case walk.Cache_NotACache:
			nRoutines--
			if nRoutines == 0 {
				close(filenameChan)
			}
		default:
			logger.ShouldNotHappen("only traverse the bloom folder")
		}
	}

	if len(fileNames) == 0 {
		msg := fmt.Sprint("No files found to check in", config.PathToIndex(chain))
		return errors.New(msg), false
	}

	sort.Slice(fileNames, func(i, j int) bool {
		return fileNames[i] < fileNames[j]
	})

	cacheManifest, err := manifest.LoadManifest(chain, opts.PublisherAddr, manifest.LocalCache)
	if err != nil {
		return err, false
	}

	remoteManifest, err := manifest.LoadManifest(chain, opts.PublisherAddr, manifest.TempContract)
	if err != nil {
		return err, false
	}
	historyFile := filepath.Join(config.PathToRootConfig(), "unchained.txt")
	saved := history.FromHistory(historyFile, "headerVersion")
	defer func() {
		_ = history.ToHistory(historyFile, "headerVersion", saved)
	}()
	_ = history.ToHistory(historyFile, "headerVersion", remoteManifest.Version)
	logger.InfoTable("Existing version:", saved)
	logger.InfoTable("Remote version:", remoteManifest.Version)

	// a string array of the actual files in the index
	fnArray := []string{}
	for _, fileName := range fileNames {
		rng := ranges.RangeFromFilename(fileName)
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

	allowMissing := config.GetScrape(chain).AllowMissing

	seq := types.ReportCheck{Reason: "Filenames sequential"}
	if err := opts.CheckSequential(fileNames, cacheArray, remoteArray, allowMissing, &seq); err != nil {
		return err, false
	}
	reports = append(reports, seq)

	intern := types.ReportCheck{Reason: "Internally consistent"}
	if err := opts.CheckInternal(fileNames, blockNums, &intern); err != nil {
		return err, false
	}
	reports = append(reports, intern)

	version := types.ReportCheck{Reason: "Correct version"}
	if err := opts.CheckVersion(fileNames, blockNums, &version); err != nil {
		return err, false
	}
	reports = append(reports, version)

	con := types.ReportCheck{Reason: "Consistent hashes"}
	if err := opts.CheckHashes(cacheManifest, remoteManifest, &con); err != nil {
		return err, false
	}
	reports = append(reports, con)

	sizes := types.ReportCheck{Reason: "Check file sizes"}
	if err := opts.CheckSizes(fileNames, blockNums, cacheManifest, remoteManifest, &sizes); err != nil {
		return err, false
	}
	reports = append(reports, sizes)

	// are all the hashes present?
	contentCheck := types.ReportCheck{}
	contentCheck.Reason = "Remote manifest contents"
	if err := opts.CheckManContents(remoteManifest, &contentCheck); err != nil {
		return err, false
	}
	reports = append(reports, contentCheck)

	contentCheck.Reason = "Local manifest contents"
	if err := opts.CheckManContents(remoteManifest, &contentCheck); err != nil {
		return err, false
	}
	reports = append(reports, contentCheck)

	// compare with çached manifest with files on disc
	d2c := types.ReportCheck{Reason: "Disc files to cached manifest"}
	if err := opts.CheckManifest(fnArray, cacheArray, &d2c); err != nil {
		return err, false
	}
	reports = append(reports, d2c)

	// compare with remote manifest with files on disc
	d2r := types.ReportCheck{Reason: "Disc files to remote manifest"}
	if err := opts.CheckManifest(fnArray, remoteArray, &d2r); err != nil {
		return err, false
	}
	reports = append(reports, d2r)

	// compare remote manifest to cached manifest
	r2c := types.ReportCheck{Reason: "Remote manifest to cached manifest"}
	if err := opts.CheckManifest(remoteArray, cacheArray, &r2c); err != nil {
		return err, false
	}
	reports = append(reports, r2c)

	if opts.Deep {
		deep := types.ReportCheck{Reason: "Deep checks for " + opts.Mode}
		if err := opts.CheckDeep(cacheManifest, &deep); err != nil {
			return err, false
		}
		reports = append(reports, deep)
	}

	nFailed := 0
	for i := 0; i < len(reports); i++ {
		reports[i].FailedCnt = reports[i].CheckedCnt - reports[i].PassedCnt
		if reports[i].FailedCnt == 0 {
			reports[i].Result = "passed"
		} else {
			reports[i].Result = "failed"
			reports[i].SkippedCnt = reports[i].VisitedCnt - reports[i].CheckedCnt
		}
		nFailed += int(reports[i].FailedCnt)
	}

	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		_ = errorChan
		for _, report := range reports {
			if !silent {
				modelChan <- &report
			}
		}
	}

	err = output.StreamMany(rCtx, fetchData, opts.Globals.OutputOpts())
	return err, nFailed == 0
}
