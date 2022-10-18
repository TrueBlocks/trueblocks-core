// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"io"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/paths"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func showAppearances(walker *index.IndexWalker, path string, first bool) (bool, error) {
	var castOk bool
	var opts *ChunksOptions
	if opts, castOk = walker.GetOpts().(*ChunksOptions); !castOk {
		logger.Fatal("should not happen ==> cannot cast ChunksOptions in showAppearances")
		return false, nil
	}
	path = paths.ToIndexPath(path)

	indexChunk, err := index.NewChunkData(path)
	if err != nil {
		return false, err
	}
	defer indexChunk.Close()

	_, err = indexChunk.File.Seek(indexChunk.AppTableStart, io.SeekStart)
	if err != nil {
		return false, err
	}

	for i := 0; i < int(indexChunk.Header.AppearanceCount); i++ {
		if opts.Globals.TestMode && i > walker.MaxTests() {
			continue
		}
		obj := index.AppearanceRecord{}
		err := obj.ReadAppearance(indexChunk.File)
		if err != nil {
			return false, err
		}
		err = opts.Globals.RenderObject(obj, first && i == 0)
		if err != nil {
			return false, err
		}
	}
	return true, nil
}

func (opts *ChunksOptions) HandleAppearances(blockNums []uint64) error {
	defer opts.Globals.RenderFooter()
	err := opts.Globals.RenderHeader(types.SimpleIndexAppearance{}, &opts.Globals.Writer, opts.Globals.Format, opts.Globals.NoHeader, true)
	if err != nil {
		return err
	}

	walker := index.NewIndexWalker(
		opts.Globals.Chain,
		opts.Globals.TestMode,
		10, /* maxTests */
		opts,
		showAppearances,
		nil, /* data */
	)
	return walker.WalkIndexFiles(paths.Index_Bloom, blockNums)
}
