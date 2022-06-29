// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"strings"

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

type WalkContext struct {
	VisitFunc func(path string, data *interface{}, first bool) (bool, error)
	Data      *interface{}
}

func (opts *ChunksOptions) WalkIndexFiles(cacheType cache.CacheType, ctx WalkContext, blockNums []uint64) error {
	filenameChan := make(chan cache.IndexFileInfo)

	var nRoutines int = 1
	go cache.WalkCacheFolder(opts.Globals.Chain, cacheType, filenameChan)

	cnt := 0
	for result := range filenameChan {
		switch result.Type {
		case cache.Index_Bloom:
			skip := (opts.Globals.TestMode && cnt > maxTestItems) || !strings.HasSuffix(result.Path, ".bloom")
			if !skip && shouldDisplay(result, blockNums) {
				ok, err := ctx.VisitFunc(result.Path, ctx.Data, cnt == 0)
				if err != nil {
					return err
				}
				if ok {
					cnt++
				}
			}
		case cache.Index_Staging:
			skip := (opts.Globals.TestMode)
			if !skip && shouldDisplay(result, blockNums) {
				ok, err := ctx.VisitFunc(result.Path, ctx.Data, cnt == 0)
				if err != nil {
					return err
				}
				if ok {
					cnt++
				}
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
