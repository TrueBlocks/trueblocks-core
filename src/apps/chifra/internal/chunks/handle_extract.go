// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
)

func shouldDisplay(result cache.IndexFileInfo, blockNums []uint64) bool {
	if len(blockNums) == 0 {
		return true
	}
	hit := false
	for _, bn := range blockNums {
		h := result.Range.BlockIntersects(bn)
		hit = hit || h
		if hit {
			break
		}
	}
	return hit
}

func (opts *ChunksOptions) HandleChunksExtract(displayFunc func(path string, first bool) error, blockNums []uint64) error {
	filenameChan := make(chan cache.IndexFileInfo)

	var nRoutines int = 1
	go cache.WalkCacheFolder(opts.Globals.Chain, cache.Index_Bloom, filenameChan)

	cnt := 0
	for result := range filenameChan {
		switch result.Type {
		case cache.Index_Bloom:
			skip := opts.Globals.TestMode && cnt > 200
			if !skip && shouldDisplay(result, blockNums) {
				displayFunc(result.Path, cnt == 0)
				cnt++
			}
		case cache.None:
			nRoutines--
			if nRoutines == 0 {
				close(filenameChan)
				continue
			}
		}
	}

	return nil
}
