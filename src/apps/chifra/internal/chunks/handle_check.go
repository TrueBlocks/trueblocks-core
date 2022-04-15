// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"fmt"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *ChunksOptions) HandleChunksCheck() error {
	blocks := cache.Convert(opts.Blocks)
	filenameChan := make(chan cache.IndexFileInfo)

	var nRoutines int = 1
	go cache.WalkCacheFolder(opts.Globals.Chain, cache.Index_Bloom, filenameChan)

	filenames := []string{}
	for result := range filenameChan {
		switch result.Type {
		case cache.Index_Bloom:
			hit := false
			for _, block := range blocks {
				h := result.Range.BlockIntersects(block)
				hit = hit || h
			}
			if len(blocks) == 0 || hit {
				filenames = append(filenames, result.Path)
			}
		case cache.None:
			nRoutines--
			if nRoutines == 0 {
				close(filenameChan)
			}
		}
	}

	sort.Slice(filenames, func(i, j int) bool {
		return filenames[i] < filenames[j]
	})

	nChecks := 0
	nChecksFailed := 0
	notARange := cache.FileRange{First: utils.NOPOS, Last: utils.NOPOS}
	if len(filenames) > 0 {
		prev := notARange
		for _, filename := range filenames {
			fR, _ := cache.RangeFromFilename(filename)
			if prev == notARange {
				prev = fR
			} else {
				nChecks++
				if !fR.SequentiallyFollows(prev) {
					nChecksFailed++
					fmt.Println(fR, "does not sequentially follow", prev)
				}
			}
			prev = fR
		}
		fmt.Println(nChecksFailed, "failed checks out of", nChecks, ".")
	}

	return nil
}
