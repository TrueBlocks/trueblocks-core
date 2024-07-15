// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

func (opts *ChunksOptions) HandleStats(rCtx *output.RenderCtx, blockNums []base.Blknum) error {
	chain := opts.Globals.Chain
	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		showFinalizedStats := func(walker *walk.CacheWalker, path string, first bool) (bool, error) {
			if path != index.ToBloomPath(path) {
				return false, fmt.Errorf("should not happen in showFinalizedStats")
			}

			if s, err := GetChunkStats(chain, path); err != nil {
				return false, err

			} else {
				modelChan <- &s
			}

			return true, nil
		}

		walker := walk.NewCacheWalker(
			chain,
			opts.Globals.TestMode,
			100, /* maxTests */
			showFinalizedStats,
		)

		if err := walker.WalkBloomFilters(blockNums); err != nil {
			errorChan <- err
			rCtx.Cancel()
		}
	}

	return output.StreamMany(rCtx.Ctx, fetchData, opts.Globals.OutputOpts())
}
