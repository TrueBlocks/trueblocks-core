// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/paths"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ChunksOptions) HandleIndex(blockNums []uint64) error {
	defer opts.Globals.RenderFooter()
	err := opts.Globals.RenderHeader(types.SimpleIndex{}, &opts.Globals.Writer, opts.Globals.Format, opts.Globals.NoHeader, true)
	if err != nil {
		return err
	}

	showIndex := func(walker *index.IndexWalker, path string, first bool) (bool, error) {
		if path != paths.ToBloomPath(path) {
			logger.Fatal("should not happen ==> we're spinning through the bloom filters")
		}

		path = paths.ToIndexPath(path)
		if !file.FileExists(path) {
			// Weird case when bloom files exist, but index files don't
			return true, nil
		}

		header, err := index.ReadChunkHeader(path, true)
		if err != nil {
			return false, err
		}

		rng, err := paths.RangeFromFilenameE(path)
		if err != nil {
			return false, err
		}

		obj := types.SimpleIndex{
			Range:           rng,
			Magic:           header.Magic,
			Hash:            base.HexToHash(header.Hash.Hex()),
			AddressCount:    header.AddressCount,
			AppearanceCount: header.AppearanceCount,
			Size:            file.FileSize(path),
		}

		// TODO: Feature - customize display strings
		// opts.Globals.Format = "Magic,Hash,Size,AppearanceCount,AddressCount,Range"
		// opts.Globals.Format = "Range\tAppearanceCount\tAddressCount"
		// TODO: Fix export without arrays
		err = opts.Globals.RenderObject(obj, first)
		if err != nil {
			return false, err
		}

		return true, nil
	}

	walker := index.NewIndexWalker(
		opts.Globals.Chain,
		opts.Globals.TestMode,
		100, /* maxTests */
		showIndex,
	)
	return walker.WalkBloomFilters(blockNums)
}
