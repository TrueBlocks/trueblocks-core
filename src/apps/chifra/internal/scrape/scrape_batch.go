package scrapePkg

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"errors"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

// ScrapeBatch is called each time around the forever loop. It calls into
// HandleBlaze and writes the timestamps if there's no error.
func (bm *BlazeManager) ScrapeBatch(blocks []base.Blknum) (error, bool) {
	chain := bm.chain

	_, _ = bm.HandleBlaze(blocks)
	if len(bm.errors) > 0 {
		for _, err := range bm.errors {
			logger.Error(fmt.Sprintf("error at block %d: %v", err.block, err.err))
		}
		_ = cleanEphemeralIndexFolders(chain)
		return errors.New("encountered errors while scraping"), true
	}

	// Check to see if we missed any blocks...
	for _, block := range blocks {
		if !bm.processedMap[block] {
			for _, err := range bm.errors {
				logger.Error(fmt.Sprintf("error at block %d: %v", err.block, err.err))
			}
			// We missed a block. We need to clean up and continue
			// next time around the loop. This may happen if the
			// node returns an error for example.
			_ = cleanEphemeralIndexFolders(chain)
			return fmt.Errorf("a block (%d) was not processed", block), true
		}
	}

	// defensive programming...
	if len(blocks) != len(bm.processedMap) ||
		len(blocks) != bm.nProcessed() ||
		bm.nTimestamps != bm.nProcessed() {
		for _, err := range bm.errors {
			logger.Error(fmt.Sprintf("error at block %d: %v", err.block, err.err))
		}
		_ = cleanEphemeralIndexFolders(chain)
		return fmt.Errorf(`check failed len(blocks): %d len(map): %d nRipe: %d nUnripe: %d nProcessed: %d nTs: %d`,
			len(blocks),
			len(bm.processedMap),
			bm.nRipe,
			bm.nUnripe,
			bm.nProcessed(),
			bm.nTimestamps,
		), true
	}

	return bm.WriteTimestamps(blocks), true
}
