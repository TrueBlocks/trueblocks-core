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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config/scrapeCfg"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// HandleChunksCheck looks at three different arrays: index files on disc, manifest on disc,
// and manifest in the smart contract. It tries to check these three sources for
// cosnsistency. Smart contract rules, so it is checked more thoroughly.
func (opts *ChunksOptions) HandleChunksCheck(blockNums []uint64) error {
	// Checking only reports in JSON Mode
	opts.Globals.Format = "json"

	maxTestItems := 10
	filenameChan := make(chan cache.CacheFileInfo)

	var nRoutines int = 1
	go cache.WalkCacheFolder(context.Background(), opts.Globals.Chain, cache.Index_Bloom, nil, filenameChan)

	fileNames := []string{}
	for result := range filenameChan {
		switch result.Type {
		case cache.Index_Bloom:
			skip := (opts.Globals.TestMode && len(fileNames) > maxTestItems) || !cache.IsCacheType(result.Path, cache.Index_Bloom, true /* checkExt */)
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
		case cache.Cache_NotACache:
			nRoutines--
			if nRoutines == 0 {
				close(filenameChan)
			}
		default:
			logger.Fatal("You may only traverse the bloom folder")
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

	allowMissing := scrapeCfg.AllowMissing(opts.Globals.Chain)
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

	// compare remote manifest to cached manifest
	r2c := simpleReportCheck{Reason: "Remote Manifest to Cached Manifest"}
	if err := opts.CheckManifest(remoteArray, cacheArray, &r2c); err != nil {
		return err
	}
	reports = append(reports, r2c)

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

	// stage := simpleReportCheck{Reason: "Check staging folder"}
	// if err := opts.CheckStaging(0, allowMissing, &stage); err != nil {
	// 	return err
	// }
	// reports = append(reports, stage)

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

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawModeler], errorChan chan error) {
		for _, report := range reports {
			report := report
			modelChan <- &report
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}

// TODO: BOGUS - MUST DOCUMENT
type simpleReportCheck struct {
	Reason     string   `json:"reason"`
	CheckedCnt uint32   `json:"checkedCnt"`
	FailedCnt  uint32   `json:"failedCnt"`
	MsgStrings []string `json:"msgStrings"`
	PassedCnt  uint32   `json:"passedCnt"`
	Result     string   `json:"result"`
	SkippedCnt uint32   `json:"skippedCnt"`
	VisitedCnt uint32   `json:"visitedCnt"`
}

func (s *simpleReportCheck) Raw() *types.RawModeler {
	return nil
}

func (s *simpleReportCheck) Model(showHidden bool, format string, extraOptions map[string]any) types.Model {
	model := map[string]any{
		"reason":     s.Reason,
		"result":     s.Result,
		"checkedCnt": s.CheckedCnt,
	}
	if s.VisitedCnt > 0 {
		model["visitedCnt"] = s.VisitedCnt
	}
	if s.PassedCnt > 0 {
		model["passedCnt"] = s.PassedCnt
	}
	if s.SkippedCnt > 0 {
		model["skippedCnt"] = s.SkippedCnt
	}
	if s.FailedCnt > 0 {
		model["failedCnt"] = s.FailedCnt
	}
	if len(s.MsgStrings) > 0 {
		model["msgStrings"] = s.MsgStrings
	}

	return types.Model{
		Data: model,
		Order: []string{
			"reason",
			"result",
			"checkedCnt",
			"visitedCnt",
			"passedCnt",
			"skippedCnt",
			"failedCnt",
			"msgStrings",
		},
	}
}
