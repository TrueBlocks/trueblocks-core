// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"context"
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index/bloom"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ChunksOptions) HandleBlooms(blockNums []uint64) error {
	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawModeler], errorChan chan error) {
		showBloom := func(walker *index.CacheWalker, path string, first bool) (bool, error) {
			if path != cache.ToBloomPath(path) {
				return false, fmt.Errorf("should not happen in showBloom")
			}

			var bl bloom.ChunkBloom
			bl.ReadBloom(path)
			nInserted := 0
			for _, bl := range bl.Blooms {
				nInserted += int(bl.NInserted)
			}

			if opts.Globals.Verbose {
				displayBloom(&bl, int(opts.Globals.LogLevel))
			}

			stats, err := GetChunkStats(path)
			if err != nil {
				return false, err
			}

			s := simpleChunkBloom{
				Magic:     bl.Header.Magic,
				Hash:      bl.Header.Hash,
				Size:      int64(stats.BloomSz),
				Range:     base.FileRange{First: stats.Start, Last: stats.End},
				Count:     uint32(stats.NBlooms),
				Width:     bloom.BLOOM_WIDTH_IN_BYTES,
				NInserted: uint64(nInserted),
			}

			modelChan <- &s
			return true, nil
		}

		walker := index.NewCacheWalker(
			opts.Globals.Chain,
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

// TODO: BOGUS2 - MUST DOCUMENT
type simpleChunkBloom struct {
	Range     base.FileRange `json:"range"`
	Magic     uint16         `json:"magic"`
	Hash      base.Hash      `json:"hash"`
	Count     uint32         `json:"nBlooms"`
	NInserted uint64         `json:"nInserted"`
	Size      int64          `json:"size"`
	Width     uint64         `json:"byteWidth"`
}

func (s *simpleChunkBloom) Raw() *types.RawModeler {
	return nil
}

func (s *simpleChunkBloom) Model(showHidden bool, format string, extraOptions map[string]any) types.Model {
	return types.Model{
		Data: map[string]any{
			"range":     s.Range,
			"magic":     fmt.Sprintf("0x%x", s.Magic),
			"hash":      s.Hash,
			"nBlooms":   s.Count,
			"nInserted": s.NInserted,
			"size":      s.Size,
			"byteWidth": s.Width,
		},
		Order: []string{
			"range",
			"magic",
			"hash",
			"nBlooms",
			"nInserted",
			"size",
			"byteWidth",
		},
	}
}
