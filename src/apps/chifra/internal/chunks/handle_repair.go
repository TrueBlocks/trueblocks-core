// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
	"github.com/ethereum/go-ethereum/common"
	"github.com/ethereum/go-ethereum/crypto"
)

func (opts *ChunksOptions) repairIndex(ctx *WalkContext, path string, first bool) (bool, error) {
	changed, err := index.WriteChunkHeaderHash(opts.Globals.Chain, path, common.BytesToHash(crypto.Keccak256([]byte(version.ManifestVersion))))
	if err != nil {
		return false, err
	}

	if !changed {
		logger.Log(logger.Info, "Chunk okay", index.ToIndexPath(path))
		return true, nil
	}

	logger.Log(logger.Info, "Repaired chunk", index.ToIndexPath(path))
	return true, nil
}

func (opts *ChunksOptions) repairManifest(ctx *WalkContext, path string, first bool) (bool, error) {
	// man, err := manifest.ReadManifest(opts.Globals.Chain, manifest.FromCache)
	// if err != nil {
	// 	return false, err
	// }

	// reports := []types.ReportRepair{}
	// for _, chunk := range man.Chunks {
	// 	rng, _ := cache.RangeFromFilename(chunk.Range + ".")
	// 	if rng.BlockIntersects(blockNums[0]) {
	// 		// TODO: BOGUS - PINNING TO PINATA AND WRITING MANIFEST FILE
	// 		report := types.ReportRepair{
	// 			Status:    "Repaired Bloom",
	// 			Range:     config.GetPathToIndex(opts.Globals.Chain) + "blooms/" + chunk.Range + ".bloom",
	// 			BloomHash: string(chunk.BloomHash),
	// 		}
	// 		reports = append(reports, report)
	// 		report = types.ReportRepair{
	// 			Status:    "Repaired Index",
	// 			Range:     config.GetPathToIndex(opts.Globals.Chain) + "finalized/" + chunk.Range + ".bin",
	// 			IndexHash: string(chunk.IndexHash),
	// 		}
	// 		reports = append(reports, report)
	// 	}
	// }

	// return true, globals.RenderSlice(&opts.Globals, reports)
	return true, nil
}

func (opts *ChunksOptions) HandleRepair(blockNums []uint64) error {
	defer opts.Globals.RenderFooter()
	err := opts.Globals.RenderHeader(types.SimpleIndex{}, &opts.Globals.Writer, opts.Globals.Format, opts.Globals.ApiMode, opts.Globals.NoHeader, true)
	if err != nil {
		return err
	}

	ctx := WalkContext{
		VisitFunc: opts.repairIndex,
	}
	if opts.Mode == "manifest" {
		ctx.VisitFunc = opts.repairManifest
	}

	return opts.WalkIndexFiles(&ctx, cache.Index_Bloom, blockNums)
}
