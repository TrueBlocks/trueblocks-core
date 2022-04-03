// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	bloomPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/bloom"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
)

func (opts *ChunksOptions) showBloom(path string, first bool) {
	if opts.Globals.TestMode {
		r, _ := cache.RangeFromFilename(path)
		if r.First > 1 {
			return
		}
	}

	var bloom bloomPkg.BloomFilter
	bloom.ReadBloomFilter(path)
	bloom.DisplayBloom(int(opts.Globals.LogLevel))
}
