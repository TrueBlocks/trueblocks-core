// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib/manifest"
)

func (opts *ChunksOptions) HandleChunksExtractPins() error {
	results, err := manifest.GetPinList(opts.Globals.Chain)
	if err != nil {
		return err
	}

	for i, r := range results {
		if opts.Globals.TestMode && i > maxTestItems {
			continue
		}
		err := opts.Globals.RenderObject(r, i == 0)
		if err != nil {
			return err
		}
	}
	return nil
}
