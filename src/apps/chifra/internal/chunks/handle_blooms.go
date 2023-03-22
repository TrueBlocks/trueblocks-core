// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index/bloom"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ChunksOptions) HandleBlooms(blockNums []uint64) error {
	defer opts.Globals.RenderFooter()
	err := opts.Globals.RenderHeader(types.SimpleBloom{}, &opts.Globals.Writer, opts.Globals.Format, opts.Globals.NoHeader, true)
	if err != nil {
		return err
	}

	showBloom := func(walker *index.IndexWalker, path string, first bool) (bool, error) {
		if path != cache.ToBloomPath(path) {
			logger.Fatal("should not happen ==> we're spinning through the bloom filters")
		}

		var bl bloom.ChunkBloom
		bl.ReadBloom(path)

		if opts.Globals.Verbose {
			displayBloom(&bl, int(opts.Globals.LogLevel))
		}

		// TODO: Fix export without arrays
		stats := NewChunkStats(path)
		obj := NewSimpleBloom(stats, bl)
		err := opts.Globals.RenderObject(obj, first)
		if err != nil {
			return false, err
		}
		return true, nil
	}

	walker := index.NewIndexWalker(
		opts.Globals.Chain,
		opts.Globals.TestMode,
		10, /* maxTests */
		showBloom,
	)
	return walker.WalkBloomFilters(blockNums)
}

func displayBloom(bl *bloom.ChunkBloom, verbose int) {
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
	fmt.Println("byteWidth:", bloom.BLOOM_WIDTH_IN_BYTES)
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

func NewSimpleBloom(stats types.ReportChunks, bl bloom.ChunkBloom) types.SimpleBloom {
	nInserted := 0
	for _, bl := range bl.Blooms {
		nInserted += int(bl.NInserted)
	}

	var ret types.SimpleBloom
	ret.Magic = bl.Header.Magic
	ret.Hash = bl.Header.Hash
	ret.Size = stats.BloomSz
	ret.Range = base.FileRange{First: stats.Start, Last: stats.End}
	ret.Count = stats.NBlooms
	ret.Width = bloom.BLOOM_WIDTH_IN_BYTES
	ret.NInserted = uint64(nInserted)

	return ret
}
