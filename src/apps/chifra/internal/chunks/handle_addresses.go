// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"io"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ChunksOptions) showAddresses(path string, first bool) (bool, error) {
	path = index.ToIndexPath(path)

	indexChunk, err := index.NewChunkData(path)
	if err != nil {
		return false, err
	}
	defer indexChunk.Close()

	_, err = indexChunk.File.Seek(int64(index.HeaderWidth), io.SeekStart)
	if err != nil {
		return false, err
	}

	cnt := 0
	for i := 0; i < int(indexChunk.Header.AddressCount); i++ {
		if opts.Globals.TestMode && i > maxTestItems {
			continue
		}

		obj, err := indexChunk.ReadAddressRecord()
		if err != nil {
			return false, err
		}

		err = opts.Globals.RenderObject(obj, first && cnt == 0)
		if err != nil {
			return false, err
		}

		if opts.Details {
			apps, err := indexChunk.ReadAppearanceRecordsAndResetOffset(&obj)
			if err != nil {
				return false, err
			}
			for _, app := range apps {
				err = opts.Globals.RenderObject(app, false)
				if err != nil {
					return false, err
				}
			}
		}
		cnt++
	}

	return true, nil
}

func (opts *ChunksOptions) HandleAddresses(blockNums []uint64) error {
	maxTestItems = 10

	defer opts.Globals.RenderFooter()
	err := opts.Globals.RenderHeader(types.SimpleIndexAddress{}, &opts.Globals.Writer, opts.Globals.Format, opts.Globals.ApiMode, opts.Globals.NoHeader, true)
	if err != nil {
		return err
	}

	return opts.WalkChunkFiles(opts.showAddresses, blockNums)
}
