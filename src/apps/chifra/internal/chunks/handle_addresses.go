// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"context"
	"encoding/binary"
	"fmt"
	"io"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

func (opts *ChunksOptions) HandleAddresses(blockNums []uint64) error {
	chain := opts.Globals.Chain
	been_here := 0
	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawChunkAddress], errorChan chan error) {
		var showAddresses func(walker *walk.CacheWalker, path string, first bool) (bool, error)
		if opts.Globals.Verbose {
			showAddresses = func(walker *walk.CacheWalker, path string, first bool) (bool, error) {
				return opts.handleResolvedRecords1(modelChan, walker, path, first)
			}
		} else {
			showAddresses = func(walker *walk.CacheWalker, path string, first bool) (bool, error) {
				if path != index.ToBloomPath(path) {
					return false, fmt.Errorf("should not happen in showAddresses")
				}

				path = index.ToIndexPath(path)
				if !file.FileExists(path) {
					// This is okay, if the user used chifra init without the --all option. Warn them and continue
					msg := ""
					path = strings.Replace(path, config.PathToIndex(chain), "$indexPath", 1)
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

				indexChunk, err := index.OpenIndex(path, true /* check */)
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

					obj := types.SimpleAddrRecord{}
					if err := binary.Read(indexChunk.File, binary.LittleEndian, &obj); err != nil {
						return false, err
					}

					s := types.SimpleChunkAddress{
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
		}

		walker := walk.NewCacheWalker(
			chain,
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
