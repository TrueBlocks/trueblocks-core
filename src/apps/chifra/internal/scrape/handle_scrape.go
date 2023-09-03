package scrapePkg

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"fmt"
	"os"
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
)

// HandleScrape enters a forever loop and continually scrapes --block_cnt blocks
// (or less if close to the head). The forever loop pauses each round for
// --sleep seconds (or, if not close to the head, for .25 seconds).
func (opts *ScrapeOptions) HandleScrape() error {
	var err error
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode

	bm := BlazeManager{
		chain:        chain,
		timestamps:   make([]tslib.TimestampRecord, 0, opts.BlockCnt),
		processedMap: make(map[base.Blknum]bool, opts.BlockCnt),
		nProcessed:   0,
		opts:         opts,
	}
	bm.meta, err = opts.Conn.GetMetaData(testMode)
	if err != nil {
		return err
	}

	// Clean the temporary files and makes sure block zero has been processed
	if ok, err := bm.Prepare(); !ok || err != nil {
		return err
	}

	runCount := uint64(0)
	// The forever loop. Loop until the user hits Cntl+C, until runCount runs
	// out, or until the server tells us to stop.
	for {
		if bm.meta, err = opts.Conn.GetMetaData(testMode); err != nil {
			logger.Error(fmt.Sprintf("Error fetching meta data: %s. Sleeping...", err))
			goto PAUSE
		}

		if bm.meta.NextIndexHeight() > bm.meta.ChainHeight() {
			// The user may have restarted his node's sync (that is, started over).
			// In this case, the index may be ahead of the chain, so we go to sleep
			// and try again later.
			msg := fmt.Sprintf("The index (%d) is ahead of the chain (%d).",
				bm.meta.NextIndexHeight(),
				bm.meta.ChainHeight(),
			)
			logger.Error(msg)
			goto PAUSE
		}

		bm = BlazeManager{
			chain:        chain,
			opts:         opts,
			nProcessed:   0,
			timestamps:   make([]tslib.TimestampRecord, 0, opts.BlockCnt),
			processedMap: make(map[base.Blknum]bool, opts.BlockCnt),
			meta:         bm.meta,
		}

		// Adjust startBlock, blockCount, and ripeBlock for this round
		bm.startBlock = bm.meta.NextIndexHeight()
		bm.blockCount = opts.BlockCnt
		if (bm.StartBlock() + bm.BlockCount()) > bm.meta.ChainHeight() {
			bm.blockCount = (bm.meta.Latest - bm.StartBlock())
		}

		// Keep ripeBlock at least Unripe_dist behind the head (i.e., 28 blocks usually - six minutes)
		bm.ripeBlock = bm.meta.ChainHeight()
		if bm.ripeBlock > opts.Settings.Unripe_dist {
			bm.ripeBlock = bm.meta.ChainHeight() - opts.Settings.Unripe_dist
		}

		// Here we do the actual scrape for this round. If anything goes wrong, the called
		// function cleans up (i.e. remove the unstaged ripe blocks). Note that even on error,
		// we don't quit. Instead we retry in the hopes that whatever happened corrects itself.
		if err := bm.ScrapeBatch(); err != nil {
			logger.Error(colors.BrightRed, err, colors.Off)
			goto PAUSE
		}

		// Try to create chunks...
		if ok, err := bm.Consolidate(); !ok || err != nil {
			logger.Error(err)
			if !ok {
				break
			}
			goto PAUSE
		}

	PAUSE:
		// The chain frequently re-orgs. Before sleeping, we remove any unripe files so they
		// are re-queried in the next round. This is the reason for the unripePath.
		unripePath := filepath.Join(config.GetPathToIndex(chain), "unripe")
		if err = os.RemoveAll(unripePath); err != nil {
			return err
		}

		runCount++
		if opts.RunCount == 0 || runCount >= opts.RunCount {
			break
		}

		bm.pause()
	}

	return nil
}
