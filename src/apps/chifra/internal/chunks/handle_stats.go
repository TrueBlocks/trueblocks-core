// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
)

func (opts *ChunksOptions) showStats(path string, first bool) error {
	// TODO: Fix export without arrays
	obj := NewChunkStats(path)
	err := opts.Globals.RenderObject(obj, false, first)
	if err != nil {
		return err
	}
	return nil
}

type ChunkStats struct {
	Start         uint64   `json:"start"`
	End           uint64   `json:"end"`
	NAddrs        uint32   `json:"nAddrs"`
	NApps         uint32   `json:"nApps"`
	NBlocks       uint64   `json:"nBlocks"`
	NBlooms       uint32   `json:"nBlooms"`
	RecWid        uint64   `json:"recWid"`
	BloomSz       int64    `json:"bloomSz"`
	ChunkSz       int64    `json:"chunkSz"`
	AddrsPerBlock float_p3 `json:"addrsPerBlock"`
	AppsPerBlock  float_p3 `json:"appsPerBlock"`
	AppsPerAddr   float_p3 `json:"appsPerAddr"`
	Ratio         float_p3 `json:"ratio"`
}

func NewChunkStats(path string) ChunkStats {
	chunk, err := index.NewChunk(path)
	defer chunk.Close()

	if err != nil {
		fmt.Println(err)
	}
	var ret ChunkStats
	ret.Start = chunk.Range.First
	ret.End = chunk.Range.Last
	ret.NAddrs = chunk.Data.Header.AddressCount
	ret.NApps = chunk.Data.Header.AppearanceCount
	ret.NBlocks = chunk.Range.Last - chunk.Range.First + 1
	ret.NBlooms = chunk.Bloom.Count

	if ret.NBlocks > 0 {
		ret.AddrsPerBlock = float_p3(ret.NAddrs) / float_p3(ret.NBlocks)
		ret.AppsPerBlock = float_p3(ret.NApps) / float_p3(ret.NBlocks)
	}

	if ret.NAddrs > 0 {
		ret.AppsPerAddr = float_p3(ret.NApps) / float_p3(ret.NAddrs)
	}

	ret.RecWid = 4 + index.BLOOM_WIDTH_IN_BYTES

	p := strings.Replace(strings.Replace(path, ".bloom", ".bin", -1), "blooms", "finalized", -1)
	ret.BloomSz = file.FileSize(path)
	ret.ChunkSz = file.FileSize(p)
	ret.Ratio = float_p3(ret.ChunkSz) / float_p3(ret.BloomSz)

	return ret
}

type float_p3 float64

func (f float_p3) String() string {
	return fmt.Sprintf("%.3f", f)
}
