// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
)

func (opts *ChunksOptions) showBloom(path string, first bool) error {
	if opts.Globals.TestMode {
		r, _ := cache.RangeFromFilename(path)
		if r.First > 1 {
			return nil
		}
	}

	var bloom index.ChunkBloom
	index.ReadBloom(&bloom, path)
	bloom.Display(int(opts.Globals.LogLevel))
	return nil
}
