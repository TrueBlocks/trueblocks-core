// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"fmt"
	"io"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/paths"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum/common/hexutil"
)

func showAddresses(walker *index.IndexWalker, path string, first bool) (bool, error) {
	var castOk bool
	var opts *ChunksOptions
	if opts, castOk = walker.GetOpts().(*ChunksOptions); !castOk {
		logger.Fatal("should not happen ==> cannot cast ChunksOptions in showAddresses")
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

		r := types.SimpleIndexAddress{
			Address: hexutil.Encode(obj.Address.Bytes()),
			Range:   indexChunk.Range.String(),
			Offset:  obj.Offset,
			Count:   obj.Count,
		}
		if !opts.Globals.ToFile {
			err = opts.Globals.RenderObject(r, first && cnt == 0)
			if err != nil {
				return false, err
			}
		}
		cnt++

		if opts.Globals.Verbose {
			if _, err = opts.HandleDetails(&indexChunk, &obj); err != nil {
				return false, err
			}
		}
	}

	return true, nil
}

func (opts *ChunksOptions) HandleDetails(indexChunk *index.ChunkData, record *index.AddressRecord) (bool, error) {
	apps, err := indexChunk.ReadAppearanceRecordsAndResetOffset(record)
	if err != nil {
		return false, err
	}

	if !opts.Globals.ToFile {
		for _, app := range apps {
			err = opts.Globals.RenderObject(app, false)
			if err != nil {
				return false, err
			}
		}
		return true, nil
	}

	save := opts.Globals.Writer
	defer func() {
		opts.Globals.Writer = save
	}()

	b := strings.Builder{}
	opts.Globals.Writer = &b
	for _, app := range apps {
		err = opts.Globals.RenderObject(app, false)
		if err != nil {
			return false, err
		}
	}

	outPath := filepath.Join(opts.Globals.OutputFn, indexChunk.Range.String())
	file.EstablishFolder(outPath)
	outFn := filepath.Join(outPath, hexutil.Encode(record.Address.Bytes())) + ".txt"
	fmt.Printf("Wrote % 5d to %s\n", len(apps), outFn)
	file.StringToAsciiFile(outFn, b.String())
	return true, nil
}

func (opts *ChunksOptions) HandleAddresses(blockNums []uint64) error {
	defer opts.Globals.RenderFooter()
	err := opts.Globals.RenderHeader(types.SimpleIndexAddress{}, &opts.Globals.Writer, opts.Globals.Format, opts.Globals.NoHeader, true)
	if err != nil {
		return err
	}

	walker := index.NewIndexWalker(
		opts.Globals.Chain,
		opts.Globals.TestMode,
		10, /* maxTests */
		opts,
		showAddresses,
		nil, /* data */
	)
	return walker.WalkIndexFiles(paths.Index_Bloom, blockNums)
}
