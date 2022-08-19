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
	path = index.ToIndexPath(path)
	changed, err := index.WriteChunkHeaderHash(opts.Globals.Chain, path, common.BytesToHash(crypto.Keccak256([]byte(version.ManifestVersion))))
	if err != nil {
		return false, err
	}

	if !changed {
		logger.Log(logger.Info, "Chunk okay", path)
		return true, nil
	}

	logger.Log(logger.Info, "Repaired chunk", path)
	return true, nil
}

func (opts *ChunksOptions) HandleRepairIndex(blockNums []uint64) error {
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
