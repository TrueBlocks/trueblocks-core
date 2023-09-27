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

// HandleScrape enters a forever loop and continually scrapes either BlockCnt blocks
// or until the chain is caught up. It pauses for Sleep --sleep seconds between each scrape.
func (opts *ScrapeOptions) HandleScrape() error {
	var err error
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode
	origBlockCnt := opts.BlockCnt

	blazeMan := BlazeManager{
		chain:        chain,
		timestamps:   make([]tslib.TimestampRecord, 0, origBlockCnt),
		processedMap: make(map[base.Blknum]bool, origBlockCnt),
		nProcessed:   0,
		opts:         opts,
	}
	blazeMan.meta, err = opts.Conn.GetMetaData(testMode)
	if err != nil {
		return err
	}

	// Clean the temporary files and makes sure block zero has been processed
	if ok, err := opts.Prepare(); !ok || err != nil {
		return err
	}

	ripeBlock := base.Blknum(0)
	unripePath := filepath.Join(config.PathToIndex(chain), "unripe")

	// The forever loop. Loop until the user hits Cntl+C or the server tells us to stop.
	for {
		if blazeMan.meta, err = opts.Conn.GetMetaData(testMode); err != nil {
			logger.Error(fmt.Sprintf("Error fetching meta data: %s. Sleeping...", err))
			goto PAUSE
		}

		if blazeMan.meta.NextIndexHeight() > blazeMan.meta.ChainHeight() {
			// If the user is re-syncing the chain, the index may be ahead of the chain,
			// so we go to sleep and try again later.
			msg := fmt.Sprintf("The index (%d) is ahead of the chain (%d).",
				blazeMan.meta.NextIndexHeight(),
				blazeMan.meta.ChainHeight(),
			)
			logger.Error(msg)
			goto PAUSE
		}

		opts.StartBlock = blazeMan.meta.NextIndexHeight()
		opts.BlockCnt = origBlockCnt
		if (blazeMan.StartBlock() + blazeMan.BlockCount()) > blazeMan.meta.ChainHeight() {
			opts.BlockCnt = (blazeMan.meta.Latest - blazeMan.StartBlock())
		}

		// The 'ripeBlock' is the head of the chain unless the chain is further along
		// than 'UnripeDist.' If it is, the `ripeBlock` is 'UnripeDist' behind the
		// head (i.e., 28 blocks usually - six minutes)
		ripeBlock = blazeMan.meta.Latest
		if ripeBlock > opts.Settings.UnripeDist {
			ripeBlock = blazeMan.meta.Latest - opts.Settings.UnripeDist
		}

		blazeMan = BlazeManager{
			chain:        chain,
			opts:         opts,
			nProcessed:   0,
			ripeBlock:    ripeBlock,
			timestamps:   make([]tslib.TimestampRecord, 0, origBlockCnt),
			processedMap: make(map[base.Blknum]bool, origBlockCnt),
			meta:         blazeMan.meta,
		}

		// Here we do the actual scrape for this round. If anything goes wrong, the
		// function will have cleaned up (i.e. remove the unstaged ripe blocks). Note
		// that we don't quit, instead we sleep and we retry continually.
		if err := blazeMan.HandleScrapeBlaze(); err != nil {
			logger.Error(colors.BrightRed, err, colors.Off)
			goto PAUSE
		}

		// Try to create chunks...
		if ok, err := blazeMan.Consolidate(); !ok || err != nil {
			logger.Error(err)
			if !ok {
				break
			}
			goto PAUSE
		}

	PAUSE:
		// The chain frequently re-orgs. Before sleeping, we remove any unripe files so they
		// are re-queried in the next round. This is the reason for the unripePath.
		if err = os.RemoveAll(unripePath); err != nil {
			return err
		}

		blazeMan.Pause()
	}

	return nil
}
