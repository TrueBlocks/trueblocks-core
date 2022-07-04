// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ChunksOptions) HandlePins(blockNums []uint64) error {
	maxTestItems = 10

	defer opts.Globals.RenderFooter()
	err := opts.Globals.RenderHeader(types.SimpleChunkRecord{}, &opts.Globals.Writer, opts.Globals.Format, opts.Globals.ApiMode, opts.Globals.NoHeader, true)
	if err != nil {
		return err
	}

	source := manifest.FromCache
	if opts.Remote {
		source = manifest.FromContract
	}

	man, err := manifest.ReadManifest(opts.Globals.Chain, source)
	if err != nil {
		return err
	}

	sort.Slice(man.Chunks, func(i, j int) bool {
		iPin := man.Chunks[i]
		jPin := man.Chunks[j]
		return iPin.Range < jPin.Range
	})

	pinList := make([]types.SimpleChunkRecord, len(man.Chunks))
	for i := range man.Chunks {
		pinList[i].Range = man.Chunks[i].Range
		pinList[i].BloomHash = string(man.Chunks[i].BloomHash)
		pinList[i].IndexHash = string(man.Chunks[i].IndexHash)
	}

	for i, r := range pinList {
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
