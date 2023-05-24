// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"context"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

var sourceMap = map[bool]manifest.Source{
	false: manifest.FromCache,
	true:  manifest.FromContract,
}

func (opts *ChunksOptions) HandleManifest(blockNums []uint64) error {
	testMode := opts.Globals.TestMode
	man, err := manifest.ReadManifest(opts.Globals.Chain, sourceMap[opts.Remote])
	if err != nil {
		return err
	}

	ctx := context.Background()
	if opts.Globals.Format == "txt" || opts.Globals.Format == "csv" {
		fetchData := func(modelChan chan types.Modeler[types.RawChunkRecord], errorChan chan error) {
			for index, chunk := range man.Chunks {
				if testMode && index > 10 {
					continue
				}
				s := types.SimpleChunkRecord{
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
		fetchData := func(modelChan chan types.Modeler[types.RawModeler], errorChan chan error) {
			if opts.Globals.TestMode {
				if len(man.Chunks) > 10 {
					man.Chunks = man.Chunks[:10]
				}
				man.Schemas = "--testing-hash--"
			}

			s := simpleManifest{
				Version: man.Version,
				Chain:   man.Chain,
				Schemas: man.Schemas,
			}
			for _, chunk := range man.Chunks {
				s.Chunks = append(s.Chunks, types.SimpleChunkRecord{
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
