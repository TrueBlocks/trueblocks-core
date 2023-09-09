// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"context"
	"errors"
	"fmt"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config/scrapeCfg"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

// HandleCheck looks at three different arrays: index files on disc, manifest on disc,
// and manifest in the smart contract. It tries to check these three sources for
// cosnsistency. Smart contract rules, so it is checked more thoroughly.
func (opts *ChunksOptions) HandleCheck(blockNums []uint64) error {
	chain := opts.Globals.Chain

	maxTestItems := 10
	filenameChan := make(chan walk.CacheFileInfo)

	var nRoutines = 1
	go walk.WalkCacheFolder(context.Background(), chain, walk.Index_Bloom, nil, filenameChan)

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
			logger.Fatal("You may only traverse the bloom folder")
		}
	}

	if len(fileNames) == 0 {
		msg := fmt.Sprint("No files found to check in", config.GetPathToIndex(chain))
		return errors.New(msg)
	}

	sort.Slice(fileNames, func(i, j int) bool {
		return fileNames[i] < fileNames[j]
	})

	cacheManifest, err := manifest.ReadManifest(chain, base.HexToAddress(opts.Publisher), manifest.FromCache)
	if err != nil {
		return err
	}

	remoteManifest, err := manifest.ReadManifest(chain, base.HexToAddress(opts.Publisher), manifest.FromContract)
	if err != nil {
		return err
	}

	// a string array of the actual files in the index
	fnArray := []string{}
	for _, fileName := range fileNames {
		rng := base.RangeFromFilename(fileName)
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

	reports := []simpleReportCheck{}

	s, _ := scrapeCfg.GetSettings(chain, "blockScrape.toml", nil)
	allowMissing := s.Allow_missing

	seq := simpleReportCheck{Reason: "Filenames sequential"}
	if err := opts.CheckSequential(fileNames, cacheArray, remoteArray, allowMissing, &seq); err != nil {
		return err
	}
	reports = append(reports, seq)

	intern := simpleReportCheck{Reason: "Internally consistent"}
	if err := opts.CheckInternal(fileNames, blockNums, &intern); err != nil {
		return err
	}
	reports = append(reports, intern)

	con := simpleReportCheck{Reason: "Consistent hashes"}
	if err := opts.CheckHashes(cacheManifest, remoteManifest, &con); err != nil {
		return err
	}
	reports = append(reports, con)

	sizes := simpleReportCheck{Reason: "Check file sizes"}
	if err := opts.CheckSizes(fileNames, blockNums, cacheManifest, remoteManifest, &sizes); err != nil {
		return err
	}
	reports = append(reports, sizes)

	// compare with çached manifest with files on disc
	d2c := simpleReportCheck{Reason: "Disc Files to Cached Manifest"}
	if err := opts.CheckManifest(fnArray, cacheArray, &d2c); err != nil {
		return err
	}
	reports = append(reports, d2c)

	// compare with remote manifest with files on disc
	d2r := simpleReportCheck{Reason: "Disc Files to Remote Manifest"}
	if err := opts.CheckManifest(fnArray, remoteArray, &d2r); err != nil {
		return err
	}
	reports = append(reports, d2r)

	// compare remote manifest to cached manifest
	r2c := simpleReportCheck{Reason: "Remote Manifest to Cached Manifest"}
	if err := opts.CheckManifest(remoteArray, cacheArray, &r2c); err != nil {
		return err
	}
	reports = append(reports, r2c)

	// we only check the stage if it exists
	stagePath := index.ToStagingPath(config.GetPathToIndex(chain) + "staging")
	stageFn, _ := file.LatestFileInFolder(stagePath)
	if file.FileExists(stageFn) {
		stage := simpleReportCheck{Reason: "Check staging folder"}
		if err := opts.CheckStaging(0, allowMissing, &stage); err != nil {
			return err
		}
		reports = append(reports, stage)
	}

	if opts.Deep {
		deep := simpleReportCheck{Reason: "Deep checks for " + opts.Mode}
		if err := opts.CheckDeep(cacheManifest, &deep); err != nil {
			return err
		}
		reports = append(reports, deep)
	}

	for i := 0; i < len(reports); i++ {
		reports[i].FailedCnt = reports[i].CheckedCnt - reports[i].PassedCnt
		if reports[i].FailedCnt == 0 {
			reports[i].Result = "passed"
		} else {
			reports[i].Result = "failed"
			reports[i].SkippedCnt = reports[i].VisitedCnt - reports[i].CheckedCnt
		}
	}

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawModeler], errorChan chan error) {
		for _, report := range reports {
			report := report
			modelChan <- &report
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}
