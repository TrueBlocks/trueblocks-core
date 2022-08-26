// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"log"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
)

// TODO: This should accept unresolved block ranges, not lists of block numbers
// This routine accepts the 'resolved' block numbers. If, instead, it it received the unresolved block ranges
// from the command line, it would be much more efficient. Using resolved block numbers means we have to
// provide a block range that is fine-grained enough to hit on every file inside the range. For example, if
// there are 100 files in the range 100000-200000, we need to create block numbers that cover every
// eventuallity. If on of the files has a two block range, we need to generate 50,000 block numbers. If we
// used the range on the command line instead we'd only have to intersect one range.
func shouldDisplay(result cache.IndexFileInfo, blockNums []uint64) bool {
	if len(blockNums) == 0 {
		return true
	}
	hit := false
	for _, bn := range blockNums {
		h := result.Range.IntersectsB(bn)
		hit = hit || h
		if hit {
			break
		}
	}
	return hit
}

type WalkContext struct {
	VisitFunc func(ctx *WalkContext, path string, first bool) (bool, error)
	Data      interface{}
}

func (opts *ChunksOptions) WalkIndexFiles(ctx *WalkContext, cacheType cache.CacheType, blockNums []uint64) error {
	filenameChan := make(chan cache.IndexFileInfo)

	var nRoutines int = 1
	go cache.WalkCacheFolder(opts.Globals.Chain, cacheType, filenameChan)

	cnt := 0
	for result := range filenameChan {
		switch result.Type {
		case cache.Index_Bloom:
			skip := (opts.Globals.TestMode && cnt > maxTestItems) || !strings.HasSuffix(result.Path, ".bloom")
			if !skip && shouldDisplay(result, blockNums) {
				ok, err := ctx.VisitFunc(ctx, result.Path, cnt == 0)
				if err != nil {
					return err
				}
				if ok {
					cnt++
				} else {
					return nil
				}
			}
		case cache.Index_Staging:
			skip := (opts.Globals.TestMode)
			if !skip && shouldDisplay(result, blockNums) {
				ok, err := ctx.VisitFunc(ctx, result.Path, cnt == 0)
				if err != nil {
					return err
				}
				if ok {
					cnt++
				} else {
					return nil
				}
			}
		case cache.None:
			nRoutines--
			if nRoutines == 0 {
				close(filenameChan)
				continue
			}
		default:
			log.Fatal("You may only traverse bloom or stage folders")
		}
	}

	return nil
}
