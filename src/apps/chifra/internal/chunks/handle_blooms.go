// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ranges"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

func (opts *ChunksOptions) HandleBlooms(rCtx *output.RenderCtx, blockNums []base.Blknum) error {
	chain := opts.Globals.Chain

	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		showBloom := func(walker *walk.CacheWalker, path string, first bool) (bool, error) {
			_ = walker
			_ = first
			if path != index.ToBloomPath(path) {
				return false, fmt.Errorf("should not happen in showBloom")
			}

			// Use optimized metadata reader instead of loading entire bloom filter
			header, count, nInserted, err := index.ReadBloomMetadata(path, true, opts.Globals.Verbose)
			if err != nil {
				return false, err
			}

			if opts.Globals.Verbose {
				// For verbose mode, we still need the full bloom for display
				var bl index.Bloom
				_ = bl.Read(path)
				displayBloom(&bl, 1)
			}

			stats, err := GetChunkStats(chain, path)
			if err != nil {
				return false, err
			}

			rng, err := ranges.RangeFromFilenameE(path)
			if err != nil {
				return false, err
			}
			s := types.ChunkBloom{
				Magic:     fmt.Sprintf("0x%x", header.Magic),
				Hash:      header.Hash,
				FileSize:  stats.BloomSz,
				Range:     rng.String(),
				NBlooms:   uint64(count),
				ByteWidth: index.BLOOM_WIDTH_IN_BYTES,
				NInserted: nInserted,
			}
			rd := tslib.RangeToBounds(chain, &rng)
			s.RangeDates = &rd

			modelChan <- &s
			return true, nil
		}

		walker := walk.NewCacheWalker(
			chain,
			opts.Globals.TestMode,
			10, /* maxTests */
			showBloom,
		)
		if err := walker.WalkBloomFilters(blockNums); err != nil {
			errorChan <- err
			rCtx.Cancel()
		}
	}

	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOpts())
}

func displayBloom(bl *index.Bloom, verbose int) {
	var bytesPerLine = (2048 / 16) /* 128 */
	if verbose > 0 && verbose <= 4 {
		bytesPerLine = 32
	}

	nInserted := uint32(0)
	for i := uint32(0); i < bl.Count; i++ {
		nInserted += bl.Blooms[i].NInserted
	}
	fmt.Println("range:", bl.Range)
	fmt.Println("nBlooms:", bl.Count)
	fmt.Println("byteWidth:", index.BLOOM_WIDTH_IN_BYTES)
	fmt.Println("nInserted:", nInserted)
	if verbose > 0 {
		for i := uint32(0); i < bl.Count; i++ {
			for j := 0; j < len(bl.Blooms[i].Bytes); j++ {
				if (j % bytesPerLine) == 0 {
					if j != 0 {
						fmt.Println()
					}
				}
				ch := bl.Blooms[i].Bytes[j]
				str := fmt.Sprintf("%08b", ch)
				fmt.Printf("%s ", strings.Replace(str, "0", ".", -1))
			}
		}
		fmt.Println()
	}
}
