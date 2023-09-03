package scrapePkg

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"errors"
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
)

// ScrapeBatch is called each time around the forever loop prior to calling into
// Blaze to actually scrape the blocks.
func (bm *BlazeManager) ScrapeBatch() (error, bool) {
	chain := bm.chain

	// Do the actual scrape, wait until it finishes, clean up and return on failure
	if _, err := bm.HandleBlaze(); err != nil {
		_ = index.CleanTemporaryFolders(config.GetPathToIndex(chain), false)
		return err, true
	}

	start := bm.StartBlock()
	end := bm.StartBlock() + bm.BlockCount()

	for bn := start; bn < end; bn++ {
		if !bm.processedMap[bn] {
			// At least one block was not processed. This would only happen in the event of an
			// error, so clean up, report the error and return. The loop will repeat.
			_ = index.CleanTemporaryFolders(config.GetPathToIndex(chain), false)
			msg := fmt.Sprintf("A block %d was not processed%s", bn, strings.Repeat(" ", 50))
			return errors.New(msg), true
		}
	}

	return bm.SaveTimestamps(end), true
}
