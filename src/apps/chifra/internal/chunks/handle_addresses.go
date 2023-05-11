// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"context"
	"fmt"
	"io"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ChunksOptions) HandleAddresses(blockNums []uint64) error {
	been_here := 0
	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawModeler], errorChan chan error) {
		showAddresses := func(walker *index.CacheWalker, path string, first bool) (bool, error) {
			if path != cache.ToBloomPath(path) {
				return false, fmt.Errorf("should not happen in showAddresses")
			}

			path = cache.ToIndexPath(path)
			if !file.FileExists(path) {
				// This is okay, if the user used chifra init without the --all option. Warn them and continue
				msg := ""
				path = strings.Replace(path, config.GetPathToIndex(opts.Globals.Chain), "$indexPath", 1)
				if been_here < 3 {
					msg = fmt.Sprintf("index file %s does not exist. Run 'chifra init --all' to create it.", path)
				} else if been_here == 3 {
					msg = fmt.Sprintf("index file %s does not exist. Warnings turned off...", path)
				}
				if msg != "" {
					errorChan <- fmt.Errorf(msg)
				}
				been_here++
				return true, nil
			}

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

				s := simpleChunkAddress{
					Address: obj.Address,
					Range:   indexChunk.Range.String(),
					Offset:  uint64(obj.Offset),
					Count:   uint64(obj.Count),
				}

				modelChan <- &s
				cnt++
			}
			return true, nil
		}

		walker := index.NewCacheWalker(
			opts.Globals.Chain,
			opts.Globals.TestMode,
			10, /* maxTests */
			showAddresses,
		)
		if err := walker.WalkBloomFilters(blockNums); err != nil {
			errorChan <- err
			cancel()
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}
