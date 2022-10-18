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
	"github.com/ethereum/go-ethereum/common/hexutil"
)

func showAddressesBelongs(walker *index.IndexWalker, path string, first bool) (bool, error) {
	var castOk bool
	var opts *ChunksOptions
	if opts, castOk = walker.GetOpts().(*ChunksOptions); !castOk {
		logger.Fatal("should not happen ==> cannot cast ChunksOptions in showAddressBelongs")
		return false, nil
	}

	path = paths.ToIndexPath(path)

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
		if opts.Globals.TestMode && i > walker.MaxTests() {
			continue
		}

		obj := index.AddressRecord{}
		err := obj.ReadAddress(indexChunk.File)
		if err != nil {
			return false, err
		}

		if opts.shouldShow(obj) {
			err = opts.Globals.RenderObject(obj, first && cnt == 0)
			if err != nil {
				return false, err
			}
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
			cnt++
		}
	}

	return true, nil
}

func (opts *ChunksOptions) shouldShow(obj index.AddressRecord) bool {
	for _, addr := range opts.Belongs {
		if hexutil.Encode(obj.Address.Bytes()) == addr {
			return true
		}
	}
	return false
}

func (opts *ChunksOptions) HandleIndexBelongs(blockNums []uint64) error {
	defer opts.Globals.RenderFooter()
	err := opts.Globals.RenderHeader(types.SimpleIndexAddressBelongs{}, &opts.Globals.Writer, opts.Globals.Format, opts.Globals.NoHeader, true)
	if err != nil {
		return err
	}

	walker := index.NewIndexWalker(
		opts.Globals.Chain,
		opts.Globals.TestMode,
		10000, /* maxTests */
		opts,
		showAddressesBelongs,
		nil, /* data */
	)
	return walker.WalkIndexFiles(paths.Index_Bloom, blockNums)
}
