// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"context"
	"fmt"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

func (opts *ChunksOptions) HandleIndex(blockNums []uint64) error {
	if len(opts.Belongs) > 0 {
		return opts.HandleIndexBelongs(blockNums)
	}

	chain := opts.Globals.Chain
	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawModeler], errorChan chan error) {
		showIndex := func(walker *walk.CacheWalker, fileName string, first bool) (bool, error) {
			if fileName != index.ToBloomPath(fileName) {
				return false, fmt.Errorf("should not happen in showIndex")
			}

			fileName = index.ToIndexPath(fileName)
			if !file.FileExists(fileName) {
				// Bloom files exist, but index files don't. It's okay.
				return true, nil
			}

			var err error
			var idx index.Index
			idx.File, err = os.OpenFile(fileName, os.O_RDONLY, 0)
			if err != nil {
				return false, err
			}
			defer idx.File.Close()

			header, err := idx.ReadHeader(config.HeaderTag(), false /* unused */)
			if err != nil {
				return false, fmt.Errorf("%w: %s", err, fileName)
			}

			rng, err := base.RangeFromFilenameE(fileName)
			if err != nil {
				return false, err
			}

			s := simpleChunkIndex{
				Range:        rng.String(),
				Magic:        fmt.Sprintf("0x%x", header.Magic),
				Hash:         base.HexToHash(header.Hash.Hex()),
				NAddresses:   uint64(header.AddressCount),
				NAppearances: uint64(header.AppearanceCount),
				Size:         uint64(file.FileSize(fileName)),
			}

			modelChan <- &s
			return true, nil
		}

		walker := walk.NewCacheWalker(
			chain,
			opts.Globals.TestMode,
			100, /* maxTests */
			showIndex,
		)
		if err := walker.WalkBloomFilters(blockNums); err != nil {
			errorChan <- err
			cancel()
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}
