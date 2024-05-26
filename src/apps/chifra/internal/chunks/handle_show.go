// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

func (opts *ChunksOptions) HandleShow(blockNums []base.Blknum) error {
	var err error
	switch opts.Mode {
	case "manifest":
		err = opts.HandleManifest(blockNums)

	case "index":
		err = opts.HandleIndex(blockNums)

	case "blooms":
		err = opts.HandleBlooms(blockNums)

	case "addresses":
		err = opts.HandleAddresses(blockNums)

	case "appearances":
		err = opts.HandleAppearances(blockNums)

	case "stats":
		err = opts.HandleStats(blockNums)

	default:
		logger.Fatal("should not happen ==> in NamesInternal")
	}
	return err
}
