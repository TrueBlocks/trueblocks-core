// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"context"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

var sourceMap = map[bool]manifest.Source{
	false: manifest.LocalCache,
	true:  manifest.FromContract,
}

func (opts *ChunksOptions) HandleManifest(blockNums []base.Blknum) error {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode
	man, err := manifest.ReadManifest(chain, opts.PublisherAddr, sourceMap[opts.Remote])
	if err != nil {
		return err
	}
	if testMode {
		if len(man.Chunks) > 10 {
			man.Chunks = man.Chunks[:10]
		}
		man.Specification = "--testing-hash--"
	}

	ctx := context.Background()
	if opts.Globals.Format == "txt" || opts.Globals.Format == "csv" {
		fetchData := func(modelChan chan types.Modeler[types.RawChunkRecord], errorChan chan error) {
			for _, chunk := range man.Chunks {
				s := types.ChunkRecord{
					Range:     chunk.Range,
					BloomHash: chunk.BloomHash,
					BloomSize: chunk.BloomSize,
					IndexHash: chunk.IndexHash,
					IndexSize: chunk.IndexSize,
				}
				modelChan <- &s
			}
		}

		return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())

	} else {
		fetchData := func(modelChan chan types.Modeler[types.RawManifest], errorChan chan error) {
			s := types.Manifest{
				Version:       man.Version,
				Chain:         man.Chain,
				Specification: man.Specification,
			}
			for _, chunk := range man.Chunks {
				s.Chunks = append(s.Chunks, types.ChunkRecord{
					Range:     chunk.Range,
					BloomHash: chunk.BloomHash,
					BloomSize: chunk.BloomSize,
					IndexHash: chunk.IndexHash,
					IndexSize: chunk.IndexSize,
				})
			}
			modelChan <- &s
		}

		return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
	}
}
