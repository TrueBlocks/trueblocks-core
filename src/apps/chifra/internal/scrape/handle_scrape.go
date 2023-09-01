package scrapePkg

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"fmt"
	"os"
	"path/filepath"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

// TODO: We should repond to non-tracing (i.e. Geth) nodes better
// TODO: Make sure we're not running acctScrape and/or pause if it's running

func (opts *ScrapeOptions) HandleScrape() error {
	chain := opts.Globals.Chain

	progress, err := opts.Conn.GetMetaData(opts.Globals.TestMode)
	if err != nil {
		return err
	}

	provider, _ := config.GetRpcProvider(chain)
	blazeOpts := BlazeOptions{
		Chain:        chain,
		NChannels:    opts.Settings.Channel_count,
		StartBlock:   opts.StartBlock,
		BlockCount:   opts.BlockCnt,
		UnripeDist:   opts.Settings.Unripe_dist,
		RpcProvider:  provider,
		AppsPerChunk: opts.Settings.Apps_per_chunk,
		NProcessed:   0,
		TsArray:      make([]tslib.TimestampRecord, 0, opts.BlockCnt),
		ProcessedMap: make(map[base.Blknum]bool, opts.BlockCnt),
	}

	if ok, err := opts.HandlePrepare(progress, &blazeOpts); !ok || err != nil {
		return err
	}

	origBlockCnt := opts.BlockCnt
	for {
		progress, err = opts.Conn.GetMetaData(opts.Globals.TestMode)
		if err != nil {
			return err
		}

		// We start the current round one block past the end of the previous round
		opts.StartBlock = utils.Max(progress.Ripe, utils.Max(progress.Staging, progress.Finalized)) + 1
		// And each round we assume we're going to process this many blocks...
		opts.BlockCnt = origBlockCnt
		if (opts.StartBlock + opts.BlockCnt) > progress.Latest {
			// ...unless we're too close to the head, then we shorten the number of blocks to process
			opts.BlockCnt = (progress.Latest - opts.StartBlock)
		}

		// The 'ripeBlock' is the head of the chain unless the chain is further along
		// than 'UnripeDist.' If it is, the `ripeBlock` is 'UnripeDist' behind the
		// head (i.e., 28 blocks usually - six minutes)
		ripeBlock := progress.Latest
		if ripeBlock > opts.Settings.Unripe_dist {
			ripeBlock = progress.Latest - opts.Settings.Unripe_dist
		}

		provider, _ := config.GetRpcProvider(chain)

		blazeOpts = BlazeOptions{
			Chain:        chain,
			NChannels:    opts.Settings.Channel_count,
			StartBlock:   opts.StartBlock,
			BlockCount:   opts.BlockCnt,
			UnripeDist:   opts.Settings.Unripe_dist,
			RpcProvider:  provider,
			AppsPerChunk: opts.Settings.Apps_per_chunk,
			NProcessed:   0,
			RipeBlock:    ripeBlock,
			TsArray:      make([]tslib.TimestampRecord, 0, opts.BlockCnt),
			ProcessedMap: make(map[base.Blknum]bool, opts.BlockCnt),
		}

		// Remove whatever's in the unripePath before running each round. We do this
		// because the chain may have re-organized (which it does frequently). This is
		// why we have an unripePath.
		unripePath := filepath.Join(config.GetPathToIndex(chain), "unripe")
		err = os.RemoveAll(unripePath)
		if err != nil {
			return err
		}

		// In some cases, the index may be already ahead of the chain tip. (For example,
		// we may be dealing with a node installation that is being re-synced, but the
		// index already exists.) In this case, we sleep for a while to allow the chain
		// to catch up.
		m := utils.Max(progress.Ripe, utils.Max(progress.Staging, progress.Finalized)) + 1
		if m > progress.Latest {
			fmt.Println(validate.Usage("The index ({0}) is ahead of the chain ({1}).", fmt.Sprintf("%d", m), fmt.Sprintf("%d", progress.Latest)))
			goto PAUSE
		}

		// Here we do the actual scrape for this round. If anything goes wrong, the
		// function will have cleaned up (i.e. remove the unstaged ripe blocks). Note
		// that we don't quit, instead we sleep and we retry continually.
		if err := opts.HandleScrapeBlaze(progress, &blazeOpts); err != nil {
			logger.Error(colors.BrightRed, err, colors.Off)
			goto PAUSE
		}
		blazeOpts.syncedReporting(base.Blknum(blazeOpts.StartBlock+blazeOpts.BlockCount), true /* force */)

		if ok, err := opts.HandleScrapeConsolidate(progress, &blazeOpts); !ok || err != nil {
			logger.Error(err)
			if !ok {
				break
			}
			goto PAUSE
		}

	PAUSE:
		opts.Pause(progress)
	}

	return nil
}

func (opts *ScrapeOptions) Pause(progressThen *rpc.MetaData) {
	// we always pause at least a quarter of a second to allow the node to 'rest'
	time.Sleep(250 * time.Millisecond)
	isDefaultSleep := opts.Sleep >= 13 && opts.Sleep <= 14
	distanceFromHead := progressThen.Latest - progressThen.Staging
	shouldSleep := !isDefaultSleep || distanceFromHead <= (2*opts.Settings.Unripe_dist)
	if shouldSleep {
		sleep := opts.Sleep
		if sleep > 1 {
			logger.Info("Sleeping for", sleep, "seconds -", distanceFromHead, "away from head.")
		}
		halfSecs := (sleep * 2) - 1 // we already slept one quarter of a second
		for i := 0; i < int(halfSecs); i++ {
			time.Sleep(time.Duration(500) * time.Millisecond)
		}
	}

}
