// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/output"
)

func (opts *ChunksOptions) HandleShow(rCtx *output.RenderCtx, blockNums []base.Blknum) error {
	var err error
	switch opts.Mode {
	case "manifest":
		err = opts.HandleManifest(rCtx, blockNums)

	case "index":
		err = opts.HandleIndex(rCtx, blockNums)

	case "blooms":
		err = opts.HandleBlooms(rCtx, blockNums)

	case "addresses":
		err = opts.HandleAddresses(rCtx, blockNums)

	case "appearances":
		err = opts.HandleAppearances(rCtx, blockNums)

	case "stats":
		err = opts.HandleStats(rCtx, blockNums)

	default:
		logger.ShouldNotHappen("in NamesInternal")
	}
	return err
}
