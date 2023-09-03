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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// HandleScrape enters a forever loop and continually scrapes --block_cnt blocks
// (or less if close to the head). The forever loop pauses each round for
// --sleep seconds (or, if not close to the head, for .25 seconds).
func (opts *ScrapeOptions) HandleScrape() error {
	var err error
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode

	// Clean the temporary files and makes sure block zero has been processed
	if ok, err := opts.Prepare(); !ok || err != nil {
		return err
	}

	runCount := uint64(0)
	// Loop until the user hits Cntl+C, until runCount runs out, or until
	// the server tells us to stop.
	for {
		// We create a new manager for each loop...
		bm := BlazeManager{
			chain: chain,
		}

		// Fetch the meta data which tells us how far along the index is.
		if bm.meta, err = opts.Conn.GetMetaData(testMode); err != nil {
			var ErrFetchingMeta = fmt.Errorf("error fetching meta data: %s", err)
			logger.Error(ErrFetchingMeta)
			goto PAUSE
		}
		// logger.Info(colors.Green+"meta data fetched"+colors.Off, bm.meta)

		// We're may be too close to the start of the chain to have ripe blocks.
		// Report no error but try again soon.
		if bm.meta.ChainHeight() < opts.Settings.Unripe_dist {
			goto PAUSE
		}
		// logger.Info(colors.Green+"ripe block found"+colors.Off, bm.meta.ChainHeight()-opts.Settings.Unripe_dist)

		// The user may have restarted his node's sync (that is, started over).
		// In this case, the index may be ahead of the chain, if so we go to
		// sleep and try again later in the hopes that the chain catches up.
		if bm.meta.NextIndexHeight() > bm.meta.ChainHeight() {
			var ErrIndexAhead = fmt.Errorf(
				"index (%d) is ahead of chain (%d)",
				bm.meta.NextIndexHeight(),
				bm.meta.ChainHeight(),
			)
			logger.Error(ErrIndexAhead)
			goto PAUSE
		}
		// logger.Info(colors.Green+"index is not ahead of chain"+colors.Off, bm.meta.NextIndexHeight())

		// Let's start a new round...
		bm = BlazeManager{
			chain:        chain,
			opts:         opts,
			nProcessed:   0,
			timestamps:   make([]tslib.TimestampRecord, 0, opts.BlockCnt),
			processedMap: make(map[base.Blknum]bool, opts.BlockCnt),
			meta:         bm.meta,
			nChannels:    int(opts.Settings.Channel_count),
		}

		// Order dependant, be careful!
		// first block to scrape (one past end of previous round).
		bm.startBlock = bm.meta.NextIndexHeight()
		// user supplied, but not so many to pass the chain tip.
		bm.blockCount = utils.Min(opts.BlockCnt, bm.meta.ChainHeight()-bm.StartBlock()+1)
		// Unripe_dist behind the chain tip.
		bm.ripeBlock = bm.meta.ChainHeight() - opts.Settings.Unripe_dist
		// logger.Info(colors.Green, bm.startBlock, bm.blockCount, bm.ripeBlock)

		// Scrape this round. Only quit on catostrophic errors. Report and sleep otherwise.
		if err, ok := bm.ScrapeBatch(); !ok || err != nil {
			logger.Error(colors.BrightRed, err, colors.Off)
			if !ok {
				break
			}
			goto PAUSE
		}
		// logger.Info(colors.Green+"scrape batch complete"+colors.Off, bm.nProcessed)

		// Consilidate a chunk (if possible). Only quit on catostrophic errors. Report and sleep otherwise.
		if ok, err := bm.Consolidate(); !ok || err != nil {
			logger.Error(colors.BrightRed, err, colors.Off)
			if !ok {
				break
			}
			goto PAUSE
		}
		// logger.Info(colors.Green+"consolidate complete"+colors.Off, bm.nProcessed)

	PAUSE:
		// If we've gotten this far, we want to clean up the unripe files (we no longer need them).
		// The chain frequently re-orgs, so we want to re-qeury these next round. This is why
		// we have an unripePath.
		unripePath := filepath.Join(config.GetPathToIndex(chain), "unripe")
		if err = os.RemoveAll(unripePath); err != nil {
			logger.Error(colors.BrightRed, err, colors.Off)
			return err
		}

		runCount++
		if opts.RunCount != 0 && runCount >= opts.RunCount {
			logger.Info("run count reached")
			break
		}

		// sleep for a bit (there's no new blocks anyway if we're caught up).
		opts.pause(bm.meta.ChainHeight() - bm.meta.StageHeight())
	}

	// We've left the loop and we're done.
	return nil
}
