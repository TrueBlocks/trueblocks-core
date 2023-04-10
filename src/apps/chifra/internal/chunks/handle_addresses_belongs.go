// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"context"
	"fmt"
	"io"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ChunksOptions) HandleIndexBelongs(blockNums []uint64) error {
	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawModeler], errorChan chan error) {
		showAddressesBelongs := func(walker *index.CacheWalker, path string, first bool) (bool, error) {
			if path != cache.ToBloomPath(path) {
				return false, fmt.Errorf("should not happen in showAddressesBelongs")
			}

			path = cache.ToIndexPath(path)
			if !file.FileExists(path) {
				// Bloom files exist, but index files don't. It's okay.
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

				s := simpleAppearanceTable{}
				err := s.AddressRecord.ReadAddress(indexChunk.File)
				if err != nil {
					return false, err
				}

				if opts.shouldShow(s.AddressRecord) {
					if s.Appearances, err = indexChunk.ReadAppearanceRecordsAndResetOffset(&s.AddressRecord); err != nil {
						return false, err
					}
					modelChan <- &s
					cnt++
				}
			}

			return true, nil
		}

		walker := index.NewCacheWalker(
			opts.Globals.Chain,
			opts.Globals.TestMode,
			10000, /* maxTests */
			showAddressesBelongs,
		)

		if err := walker.WalkBloomFilters(blockNums); err != nil {
			errorChan <- err
			cancel()
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}

// TODO: BOGUS - MUST DOCUMENT
type simpleAppearanceTable struct {
	AddressRecord index.AddressRecord      `json:"addressRecord"`
	Appearances   []index.AppearanceRecord `json:"appearances"`
}

func (s *simpleAppearanceTable) Raw() *types.RawModeler {
	return nil
}

func (s *simpleAppearanceTable) Model(showHidden bool, format string, extraOptions map[string]any) types.Model {
	return types.Model{
		Data: map[string]any{
			"addressRecord": s.AddressRecord,
			"appearances":   s.Appearances,
		},
		Order: []string{
			"addressRecord",
			"appearances",
		},
	}
}
