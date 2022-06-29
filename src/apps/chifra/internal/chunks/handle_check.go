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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ChunksOptions) HandleChunksCheck(blockNums []uint64) error {
	maxTestItems = 10
	filenameChan := make(chan cache.IndexFileInfo)

	var nRoutines int = 1
	go cache.WalkCacheFolder(opts.Globals.Chain, cache.Index_Bloom, filenameChan)

	fileNames := []string{}
	for result := range filenameChan {
		switch result.Type {
		case cache.Index_Bloom:
			skip := (opts.Globals.TestMode && len(fileNames) > maxTestItems) || !strings.HasSuffix(result.Path, ".bloom")
			if !skip {
				hit := false
				for _, block := range blockNums {
					h := result.Range.BlockIntersects(block)
					hit = hit || h
					if hit {
						break
					}
				}
				if len(blockNums) == 0 || hit {
					fileNames = append(fileNames, result.Path)
				}
			}
		case cache.None:
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

	maxTestItems = 10
	reports := []types.CheckReport{}

	reports = append(reports, types.CheckReport{Reason: "Sequential"})
	if err := opts.CheckSequential(fileNames, blockNums, &reports[0]); err != nil {
		return err
	}

	reports = append(reports, types.CheckReport{Reason: "Internally consistent"})
	if err := opts.CheckInternal(fileNames, blockNums, &reports[1]); err != nil {
		return err
	}

	manFromCache, err := manifest.ReadManifest(opts.Globals.Chain, manifest.FromCache)
	if err != nil {
		return err
	}
	remoteMan, err := manifest.ReadManifest(opts.Globals.Chain, manifest.FromContract)
	if err != nil {
		return err
	}

	// a string array of the ranges in the local manifest
	cacheArray := []string{}
	for _, chunk := range manFromCache.Chunks {
		cacheArray = append(cacheArray, chunk.Range)
	}

	// a string array of the ranges from the remote manifest
	reports = append(reports, types.CheckReport{Reason: "Consistent hashes"})
	remoteArray := []string{}
	for _, chunk := range remoteMan.Chunks {
		remoteArray = append(remoteArray, chunk.Range)
		if len(chunk.BloomHash) != 46 {
			msg := fmt.Sprintf("%s: Invalid BloomHash (%s)", chunk.Range, chunk.BloomHash)
			if len(chunk.BloomHash) == 0 {
				msg = fmt.Sprintf("%s: Empty BloomHash", chunk.Range)
			}
			reports[2].ErrorStrs = append(reports[2].ErrorStrs, msg)
		}
		if len(chunk.IndexHash) != 46 {
			msg := fmt.Sprintf("%s: Invalid IndexHash (%s)", chunk.Range, chunk.IndexHash)
			if len(chunk.BloomHash) == 0 {
				msg = fmt.Sprintf("%s: Empty IndexHash", chunk.Range)
			}
			reports[2].ErrorStrs = append(reports[2].ErrorStrs, msg)
		}
	}

	// a string array of the actual files in the index
	fnArray := []string{}
	for _, fileName := range fileNames {
		rng, _ := cache.RangeFromFilename(fileName)
		fnArray = append(fnArray, rng.String())
	}

	// compare remote manifest to cached manifest
	reports = append(reports, types.CheckReport{Reason: "Remote Manifest to Cached Manifest"})
	if err := opts.CheckManifest(remoteArray, cacheArray, &reports[3]); err != nil {
		return err
	}

	// compare with çached manifest with files on disc
	reports = append(reports, types.CheckReport{Reason: "Disc Files to Cached Manifest"})
	if err := opts.CheckManifest(fnArray, cacheArray, &reports[4]); err != nil {
		return err
	}

	// compare with remote manifest with files on disc
	reports = append(reports, types.CheckReport{Reason: "Disc Files to Remote Manifest"})
	if err := opts.CheckManifest(fnArray, remoteArray, &reports[5]); err != nil {
		return err
	}

	return globals.RenderSlice(&opts.Globals, reports)
}

// TODO: We don't check blooms
// TODO: We don't check internal consistency of the data files
// TODO:	- is every address in the index, in the bloom?
// TODO:	- are there missing blocks (if allow_missing is off)
// TODO: We don't check if Pinata has files that aren't needed
