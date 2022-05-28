// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"io"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
)

func (opts *ChunksOptions) showAppearances(path string, first bool) error {
	path = index.ToIndexPath(path)

	indexChunk, err := index.NewChunkData(path)
	if err != nil {
		return err
	}
	defer indexChunk.Close()

	_, err = indexChunk.File.Seek(indexChunk.AppTableStart, io.SeekStart)
	if err != nil {
		return err
	}

	for i := 0; i < int(indexChunk.Header.AppearanceCount); i++ {
		if opts.Globals.TestMode && i > maxTestItems {
			continue
		}
		obj, err := indexChunk.ReadAppearanceRecord()
		if err != nil {
			return err
		}
		err = opts.Globals.RenderObject(obj, first && i == 0)
		if err != nil {
			return err
		}
	}
	return nil
}
