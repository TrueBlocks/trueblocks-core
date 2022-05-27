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

type ChunkStats struct {
	Start         uint64 `json:"start"`
	End           uint64 `json:"end"`
	NAddrs        uint64 `json:"nAddrs"`
	NApps         uint64 `json:"nApps"`
	NBlocks       uint64 `json:"nBlocks"`
	NBlooms       uint64 `json:"nBlooms"`
	RecWid        uint64 `json:"recWid"`
	BloomSz       int64  `json:"bloomSz"`
	ChunkSz       int64  `json:"chunkSz"`
	AddrsPerBlock string `json:"addrsPerBlock"`
	AppsPerBlock  string `json:"appsPerBlock"`
	AppsPerAddr   string `json:"appsPerAddr"`
	Ratio         string `json:"ratio"`
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
	ret.NAddrs = uint64(chunk.Data.Header.AddressCount)
	ret.NApps = uint64(chunk.Data.Header.AppearanceCount)
	ret.NBlocks = ret.End - ret.Start + 1
	ret.NBlooms = uint64(chunk.Bloom.Count)
	if ret.NBlocks > 0 {
		ret.AddrsPerBlock = fmt.Sprintf("%.3f", float64(ret.NAddrs)/float64(ret.NBlocks))
		ret.AppsPerBlock = fmt.Sprintf("%.3f", float64(ret.NApps)/float64(ret.NBlocks))
	}
	if ret.NAddrs > 0 {
		ret.AppsPerAddr = fmt.Sprintf("%.3f", float64(ret.NApps)/float64(ret.NAddrs))
	}
	ret.RecWid = 4 + index.BLOOM_WIDTH_IN_BYTES
	ret.BloomSz = file.FileSize(path)
	p := strings.Replace(strings.Replace(path, ".bloom", ".bin", -1), "blooms", "finalized", -1)
	ret.ChunkSz = file.FileSize(p)
	ret.Ratio = fmt.Sprintf("%.3f", float64(ret.ChunkSz)/float64(ret.BloomSz))
	return ret
}
