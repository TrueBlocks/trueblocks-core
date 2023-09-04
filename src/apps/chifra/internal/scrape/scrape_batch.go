package scrapePkg

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
)

// ScrapeBatch is called each time around the forever loop. It calls into
// HandleBlaze and writes the timestamps if there's no error.
func (bm *BlazeManager) ScrapeBatch(blocks []base.Blknum) (error, bool) {
	indexPath := config.GetPathToIndex(bm.chain)

	if err, ok := bm.HandleBlaze(blocks); !ok || err != nil {
		_ = index.CleanTemporaryFolders(indexPath, false)
		return err, ok
	}

	// Check to see if we missed any blocks...
	for _, block := range blocks {
		if !bm.processedMap[block] {
			// We missed a block. We need to clean up and continue
			// next time around the loop. This may happen if the
			// node returns an error for example.
			_ = index.CleanTemporaryFolders(indexPath, false)
			return fmt.Errorf("a block (%d) was not processed", block), true
		}
	}

	return bm.WriteTimestamps(blocks), true
}

