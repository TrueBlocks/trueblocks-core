// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ChunksOptions) HandleManifest(blockNums []uint64) error {
	maxTestItems = 10

	defer opts.Globals.RenderFooter()
	err := opts.Globals.RenderHeader(types.SimpleManifest{}, &opts.Globals.Writer, opts.Globals.Format, opts.Globals.ApiMode, opts.Globals.NoHeader, true)
	if err != nil {
		return err
	}

	manFromCache, err := manifest.ReadManifest(opts.Globals.Chain, manifest.FromCache)
	if err != nil {
		return err
	}
	if opts.Globals.TestMode {
		if len(manFromCache.Chunks) > maxTestItems {
			s := len(manFromCache.Chunks) - maxTestItems - 1
			e := len(manFromCache.Chunks) - 1
			manFromCache.Chunks = manFromCache.Chunks[s:e]
		}
	}

	return opts.Globals.RenderManifest(opts.Globals.Writer, opts.Globals.Format, manFromCache)
}
