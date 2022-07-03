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

	source := manifest.FromCache
	if opts.Remote {
		source = manifest.FromContract
	}

	man, err := manifest.ReadManifest(opts.Globals.Chain, source)
	if err != nil {
		return err
	}

	if opts.Globals.TestMode {
		if len(man.Chunks) > maxTestItems {
			s := len(man.Chunks) - maxTestItems - 1
			e := len(man.Chunks) - 1
			man.Chunks = man.Chunks[s:e]
		}
	}

	return opts.Globals.RenderManifest(opts.Globals.Writer, man)
}
