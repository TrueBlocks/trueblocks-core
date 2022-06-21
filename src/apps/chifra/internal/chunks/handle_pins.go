// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ChunksOptions) HandleChunksPins() error {
	results, err := GetChunks(opts.Globals.Chain)
	if err != nil {
		return err
	}

	for i, r := range *results {
		if opts.Globals.TestMode && i > maxTestItems {
			continue
		}
		err := opts.Globals.RenderObject(r, i == 0)
		if err != nil {
			return err
		}
	}
	return nil
}

func GetChunks(chain string) (*[]types.SimpleChunkRecord, error) {
	manFromCache, err := manifest.ReadManifest(chain, manifest.FromCache)
	if err != nil {
		return nil, err
	}

	sort.Slice(manFromCache.Chunks, func(i, j int) bool {
		iPin := manFromCache.Chunks[i]
		jPin := manFromCache.Chunks[j]
		return iPin.Range < jPin.Range
	})

	pinList := make([]types.SimpleChunkRecord, len(manFromCache.Chunks))
	for i := range manFromCache.Chunks {
		pinList[i].Range = manFromCache.Chunks[i].Range
		pinList[i].BloomHash = string(manFromCache.Chunks[i].BloomHash)
		pinList[i].IndexHash = string(manFromCache.Chunks[i].IndexHash)
	}
	return &pinList, nil
}

func (opts *ChunksOptions) HandlePins(blockNums []uint64) error {
	maxTestItems = 10

	defer opts.Globals.RenderFooter()
	err := opts.Globals.RenderHeader(types.SimpleChunkRecord{}, &opts.Globals.Writer, opts.Globals.Format, opts.Globals.ApiMode, opts.Globals.NoHeader, true)
	if err != nil {
		return err
	}

	return opts.HandleChunksPins()
}
