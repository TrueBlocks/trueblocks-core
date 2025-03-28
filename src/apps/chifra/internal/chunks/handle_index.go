// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ranges"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

func (opts *ChunksOptions) HandleIndex(rCtx *output.RenderCtx, blockNums []base.Blknum) error {
	if len(opts.Belongs) > 0 {
		return opts.HandleIndexBelongs(rCtx, blockNums)
	}

	chain := opts.Globals.Chain
	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		showIndex := func(walker *walk.CacheWalker, fileName string, first bool) (bool, error) {
			_ = walker
			_ = first
			if fileName != index.ToBloomPath(fileName) {
				return false, fmt.Errorf("should not happen in showIndex")
			}

			fileName = index.ToIndexPath(fileName)
			if !file.FileExists(fileName) {
				// Bloom files exist, but index files don't. It's okay.
				return true, nil
			}

			indexChunk, err := index.OpenIndex(fileName, true /* check */)
			if err != nil {
				return false, err
			}
			defer indexChunk.File.Close()

			rng, err := ranges.RangeFromFilenameE(fileName)
			if err != nil {
				return false, err
			}

			s := types.ChunkIndex{
				Range:        rng.String(),
				Magic:        fmt.Sprintf("0x%x", indexChunk.Header.Magic),
				Hash:         indexChunk.Header.Hash,
				NAddresses:   uint64(indexChunk.Header.AddressCount),
				NAppearances: uint64(indexChunk.Header.AppearanceCount),
				Size:         uint64(file.FileSize(fileName)),
			}
			rd := tslib.RangeToBounds(chain, &rng)
			s.RangeDates = &rd

			modelChan <- &s
			return true, nil
		}

		walker := walk.NewCacheWalker(
			chain,
			opts.Globals.TestMode,
			100, /* maxTests */
			showIndex,
		)
		if err := walker.WalkBloomFilters(blockNums); err != nil {
			errorChan <- err
			rCtx.Cancel()
		}
	}

	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOpts())
}
