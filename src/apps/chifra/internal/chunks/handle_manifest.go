// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ChunksOptions) HandleChunksManifest() error {
	obj, err := manifest.FromLocalFile(opts.Globals.Chain)
	if err != nil {
		return err
	}
	if opts.Globals.TestMode {
		if len(obj.Chunks) > maxTestItems {
			s := len(obj.Chunks) - maxTestItems - 1
			e := len(obj.Chunks) - 1
			obj.Chunks = obj.Chunks[s:e]
		}
	}

	return opts.Globals.RenderManifest(opts.Globals.Writer, opts.Globals.Format, obj)
}

func (opts *ChunksOptions) HandleManifest(blockNums []uint64) error {
	maxTestItems = 10

	defer opts.Globals.RenderFooter()
	err := opts.Globals.RenderHeader(types.SimpleManifest{}, &opts.Globals.Writer, opts.Globals.Format, opts.Globals.ApiMode, opts.Globals.NoHeader, true)
	if err != nil {
		return err
	}

	return opts.HandleChunksManifest()
}
