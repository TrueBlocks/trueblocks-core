// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"context"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
)

func (opts *ChunksOptions) HandleShow(rCtx output.RenderCtx, blockNums []base.Blknum) error {
	_ = context.TODO()
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
		logger.Fatal("should not happen ==> in NamesInternal")
	}
	return err
}
