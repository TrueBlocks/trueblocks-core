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
	fetchData := func(modelChan chan types.Modeler[RawAppearanceTable], errorChan chan error) {
		showAddressesBelongs := func(walker *index.IndexWalker, path string, first bool) (bool, error) {
			if path != cache.ToBloomPath(path) {
				return false, fmt.Errorf("should not happen in showFinalizedStats")
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

				s := SimpleAppearanceTable{}
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

		walker := index.NewIndexWalker(
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

type RawAppearanceTable interface {
}

type SimpleAppearanceTable struct {
	AddressRecord index.AddressRecord
	Appearances   []index.AppearanceRecord
}

func (s *SimpleAppearanceTable) Raw() *RawAppearanceTable {
	return nil
}

func (s *SimpleAppearanceTable) Model(showHidden bool, format string, extraOptions map[string]any) types.Model {
	model := map[string]any{
		"addressRecord": s.AddressRecord,
		"appearances":   s.Appearances,
	}
	order := []string{
		"addressRecord",
		"appearances",
	}
	return types.Model{
		Data:  model,
		Order: order,
	}
}
