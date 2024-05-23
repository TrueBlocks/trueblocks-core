// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"context"
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

func (opts *ChunksOptions) HandleBlooms(blockNums []base.Blknum) error {
	chain := opts.Globals.Chain

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.ChunkBloom], errorChan chan error) {
		showBloom := func(walker *walk.CacheWalker, path string, first bool) (bool, error) {
			if path != index.ToBloomPath(path) {
				return false, fmt.Errorf("should not happen in showBloom")
			}

			var bl index.Bloom
			_ = bl.Read(path)
			nInserted := 0
			for _, bl := range bl.Blooms {
				nInserted += int(bl.NInserted)
			}

			if opts.Globals.Verbose {
				displayBloom(&bl, 1)
			}

			stats, err := GetChunkStats(chain, path)
			if err != nil {
				return false, err
			}

			s := types.ChunkBloom{
				Magic:     fmt.Sprintf("0x%x", bl.Header.Magic),
				Hash:      bl.Header.Hash,
				Size:      stats.BloomSz,
				Range:     base.RangeFromFilename(path).String(),
				NBlooms:   stats.NBlooms,
				ByteWidth: index.BLOOM_WIDTH_IN_BYTES,
				NInserted: uint64(nInserted),
			}

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
			cancel()
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
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
