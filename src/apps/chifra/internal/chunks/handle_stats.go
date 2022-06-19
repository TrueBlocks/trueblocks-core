// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ChunksOptions) showStats(path string, first bool) (bool, error) {
	// TODO: Fix export without arrays
	obj := NewChunkStats(path)
	err := opts.Globals.RenderObject(obj, first)
	if err != nil {
		return false, err
	}
	return true, nil
}

func NewChunkStats(path string) types.SimpleChunkStats {
	chunk, err := index.NewChunk(path)
	defer chunk.Close()

	if err != nil {
		fmt.Println(err)
	}
	var ret types.SimpleChunkStats
	ret.Start = chunk.Range.First
	ret.End = chunk.Range.Last
	ret.NAddrs = chunk.Data.Header.AddressCount
	ret.NApps = chunk.Data.Header.AppearanceCount
	ret.NBlocks = chunk.Range.Last - chunk.Range.First + 1
	ret.NBlooms = chunk.Bloom.Count

	if ret.NBlocks > 0 {
		ret.AddrsPerBlock = float64(ret.NAddrs) / float64(ret.NBlocks)
		ret.AppsPerBlock = float64(ret.NApps) / float64(ret.NBlocks)
	}

	if ret.NAddrs > 0 {
		ret.AppsPerAddr = float64(ret.NApps) / float64(ret.NAddrs)
	}

	ret.RecWid = 4 + index.BLOOM_WIDTH_IN_BYTES

	p := strings.Replace(strings.Replace(path, ".bloom", ".bin", -1), "blooms", "finalized", -1)
	ret.BloomSz = file.FileSize(path)
	ret.ChunkSz = file.FileSize(p)
	ret.Ratio = float64(ret.ChunkSz) / float64(ret.BloomSz)

	return ret
}

func (opts *ChunksOptions) HandleStats(blockNums []uint64) error {
	defer opts.Globals.RenderFooter()
	err := opts.Globals.RenderHeader(types.SimpleChunkStats{}, &opts.Globals.Writer, opts.Globals.Format, opts.Globals.ApiMode, opts.Globals.NoHeader, true)
	if err != nil {
		return err
	}

	return opts.WalkChunkFiles(opts.showStats, blockNums)
}
