// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ChunksOptions) HandleChunksPins() error {
	results, err := manifest.GetChunkList(opts.Globals.Chain)
	if err != nil {
		return err
	}

	for i, r := range results {
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

func (opts *ChunksOptions) HandlePins(blockNums []uint64) error {
	maxTestItems = 10

	defer opts.Globals.RenderFooter()
	err := opts.Globals.RenderHeader(types.SimpleChunk{}, &opts.Globals.Writer, opts.Globals.Format, opts.Globals.ApiMode, opts.Globals.NoHeader, true)
	if err != nil {
		return err
	}

	return opts.HandleChunksPins()
}
