// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/ranges"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/types"
)

func (opts *ChunksOptions) HandleManifest(rCtx *output.RenderCtx, blockNums []base.Blknum) error {
	_ = blockNums
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode
	source := manifest.LocalCache
	if opts.Remote {
		source = manifest.FromContract
	}
	man, err := manifest.LoadManifest(chain, opts.PublisherAddr, source)
	if err != nil {
		return err
	}
	if testMode {
		if len(man.Chunks) > 10 {
			man.Chunks = man.Chunks[:10]
		}
		man.Specification = "--testing-hash--"
	}

	if opts.Globals.Format == "txt" || opts.Globals.Format == "csv" {
		fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
			_ = errorChan
			for _, chunk := range man.Chunks {
				rng := ranges.RangeFromRangeString(chunk.Range)
				s := types.ChunkRecord{
					Range:     rng.String(),
					BloomHash: chunk.BloomHash,
					BloomSize: chunk.BloomSize,
					IndexHash: chunk.IndexHash,
					IndexSize: chunk.IndexSize,
				}
				rd := tslib.RangeToBounds(chain, &rng)
				s.RangeDates = &rd
				modelChan <- &s
			}
		}

		return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOpts())

	} else {
		fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
			_ = errorChan
			s := types.Manifest{
				Version:       man.Version,
				Chain:         man.Chain,
				Specification: man.Specification,
			}
			for _, chunk := range man.Chunks {
				rng := ranges.RangeFromRangeString(chunk.Range)
				ch := types.ChunkRecord{
					Range:     rng.String(),
					BloomHash: chunk.BloomHash,
					BloomSize: chunk.BloomSize,
					IndexHash: chunk.IndexHash,
					IndexSize: chunk.IndexSize,
				}
				rd := tslib.RangeToBounds(chain, &rng)
				ch.RangeDates = &rd
				s.Chunks = append(s.Chunks, ch)
			}
			modelChan <- &s
		}

		return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOpts())
	}
}
