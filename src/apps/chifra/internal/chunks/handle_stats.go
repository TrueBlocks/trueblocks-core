// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"context"
	"fmt"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index/bloom"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ChunksOptions) HandleStats(blockNums []uint64) error {
	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawChunkStats], errorChan chan error) {
		showFinalizedStats := func(walker *index.IndexWalker, path string, first bool) (bool, error) {
			if path != cache.ToBloomPath(path) {
				err := fmt.Errorf("should not happen in showFinalizedStats")
				errorChan <- err
				cancel()
				return false, err
			}

			chunk, err := index.NewChunk(path)
			if err != nil && !os.IsNotExist(err) {
				errorChan <- err
				cancel()
				return false, err
			}
			defer chunk.Close()

			s := types.SimpleChunkStats{
				Start:   chunk.Range.First,
				End:     chunk.Range.Last,
				NBlocks: chunk.Range.Last - chunk.Range.First + 1,
				NAddrs:  uint64(chunk.Data.Header.AddressCount),
				NApps:   uint64(chunk.Data.Header.AppearanceCount),
				NBlooms: uint64(chunk.Bloom.Count),
				BloomSz: uint64(file.FileSize(cache.ToBloomPath(path))),
				ChunkSz: uint64(file.FileSize(cache.ToIndexPath(path))),
				RecWid:  4 + bloom.BLOOM_WIDTH_IN_BYTES,
			}

			if s.NBlocks > 0 {
				s.AddrsPerBlock = float64(s.NAddrs) / float64(s.NBlocks)
				s.AppsPerBlock = float64(s.NApps) / float64(s.NBlocks)
			}

			if s.NAddrs > 0 {
				s.AppsPerAddr = float64(s.NApps) / float64(s.NAddrs)
			}

			if s.BloomSz > 0 {
				s.Ratio = float64(s.ChunkSz) / float64(s.BloomSz)
			}

			modelChan <- &s
			return true, nil
		}

		walker := index.NewIndexWalker(
			opts.Globals.Chain,
			opts.Globals.TestMode,
			100, /* maxTests */
			showFinalizedStats,
		)

		err := walker.WalkBloomFilters(blockNums)
		if err != nil {
			errorChan <- err
		}
	}

	return output.StreamMany(ctx, fetchData, output.OutputOptions{
		Writer:     opts.Globals.Writer,
		Chain:      opts.Globals.Chain,
		TestMode:   opts.Globals.TestMode,
		NoHeader:   opts.Globals.NoHeader,
		ShowRaw:    opts.Globals.ShowRaw,
		Verbose:    opts.Globals.Verbose,
		LogLevel:   opts.Globals.LogLevel,
		Format:     opts.Globals.Format,
		OutputFn:   opts.Globals.OutputFn,
		Append:     opts.Globals.Append,
		JsonIndent: "  ",
	})
}
