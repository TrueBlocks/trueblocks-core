// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"context"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ChunksOptions) HandleStats(blockNums []uint64) error {
	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[RawChunkStats], errorChan chan error) {
		showFinalizedStats := func(walker *index.IndexWalker, path string, first bool) (bool, error) {
			if path != cache.ToBloomPath(path) {
				return false, fmt.Errorf("should not happen in showFinalizedStats")
			}

			if s, err := GetChunkStats(path); err != nil {
				return false, err

			} else {
				modelChan <- &s
			}

			return true, nil
		}

		walker := index.NewIndexWalker(
			opts.Globals.Chain,
			opts.Globals.TestMode,
			100, /* maxTests */
			showFinalizedStats,
		)

		if err := walker.WalkBloomFilters(blockNums); err != nil {
			errorChan <- err
			cancel()
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}
