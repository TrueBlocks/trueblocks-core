// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"fmt"
	"strconv"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index/bloom"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ChunksOptions) showFinalizedStats(ctx *WalkContext, path string, first bool) (bool, error) {
	// TODO: Fix export without arrays
	obj := NewChunkStats(path)
	err := opts.Globals.RenderObject(obj, first)
	return err == nil, err
}

func (opts *ChunksOptions) showStagingStats(ctx *WalkContext, path string, first bool) (bool, error) {
	lines := file.AsciiFileToLines(path)
	ret := types.ReportChunks{}
	rng, err1 := cache.RangeFromFilename(path)
	if err1 != nil {
		return false, nil
	}
	ret.End = rng.Last
	ret.NApps = uint32(len(lines))
	addrMap := make(map[string]uint32, ret.NApps)
	for _, line := range lines {
		parts := strings.Split(line, "\t")
		if len(parts) > 1 && ret.Start == 0 {
			v, _ := strconv.ParseUint(parts[1], 10, 32)
			ret.Start = uint64(v)
		}
		if len(parts) > 0 {
			addrMap[parts[0]]++
		}
	}
	ret.NAddrs = uint32(len(addrMap))
	ret.NBlocks = ret.End - ret.Start + 1
	ret.NBlooms = 1
	ret.BloomSz = 1
	ret.ChunkSz = 1
	ret = finishStats(&ret)
	// TODO: Fix export without arrays
	err := opts.Globals.RenderObject(ret, first)
	return err == nil, err
}

func finishStats(stats *types.ReportChunks) types.ReportChunks {
	stats.NBlocks = stats.End - stats.Start + 1
	if stats.NBlocks > 0 {
		stats.AddrsPerBlock = float64(stats.NAddrs) / float64(stats.NBlocks)
		stats.AppsPerBlock = float64(stats.NApps) / float64(stats.NBlocks)
	}

	if stats.NAddrs > 0 {
		stats.AppsPerAddr = float64(stats.NApps) / float64(stats.NAddrs)
	}

	stats.RecWid = 4 + bloom.BLOOM_WIDTH_IN_BYTES
	if stats.BloomSz > 0 {
		stats.Ratio = float64(stats.ChunkSz) / float64(stats.BloomSz)
	}

	return *stats
}

func NewChunkStats(path string) types.ReportChunks {
	chunk, err := index.NewChunk(path)
	if err != nil {
		fmt.Println(err)
	}
	defer chunk.Close()

	var ret types.ReportChunks
	ret.Start = chunk.Range.First
	ret.End = chunk.Range.Last
	ret.NAddrs = chunk.Data.Header.AddressCount
	ret.NApps = chunk.Data.Header.AppearanceCount
	ret.NBlooms = chunk.Bloom.Count
	ret.BloomSz = file.FileSize(config.ToBloomPath(path))
	ret.ChunkSz = file.FileSize(config.ToIndexPath(path))

	return finishStats(&ret)
}

func (opts *ChunksOptions) HandleStats(blockNums []uint64) error {
	defer opts.Globals.RenderFooter()
	err := opts.Globals.RenderHeader(types.ReportChunks{}, &opts.Globals.Writer, opts.Globals.Format, opts.Globals.ApiMode, opts.Globals.NoHeader, true)
	if err != nil {
		return err
	}

	ctx := WalkContext{
		VisitFunc: opts.showFinalizedStats,
	}

	if err = opts.WalkIndexFiles(&ctx, cache.Index_Bloom, blockNums); err != nil {
		return err
	}

	if opts.Globals.Verbose {
		ctx = WalkContext{
			VisitFunc: opts.showStagingStats,
		}

		err = opts.WalkIndexFiles(&ctx, cache.Index_Staging, blockNums)
	}

	return err
}
