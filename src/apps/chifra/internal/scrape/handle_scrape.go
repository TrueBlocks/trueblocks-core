package scrapePkg

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"fmt"
	"os"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
)

// HandleScrape enters a forever loop and continually scrapes --block_cnt blocks
// (or less if close to the head). The forever loop pauses each round for
// --sleep seconds (or, if not close to the head, for .25 seconds).
func (opts *ScrapeOptions) HandleScrape() error {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode
	defer func() {
		pidPath := filepath.Join(config.PathToCache(chain), "tmp/scrape.pid")
		_ = os.Remove(pidPath)
	}()

	path := config.PathToIndex(chain)
	provider := config.GetChain(chain).RpcProvider
	if testMode {
		path = "--unchained-path--"
		provider = "--rpc-provider--"
	}
	logger.Info("Scraping:")
	logger.Info("  RPC:     ", provider)
	logger.Info("  Path:    ", path)
	logger.Info("  Settings:", config.GetScrape(chain))
	if opts.DryRun {
		return nil
	}

	var blocks = make([]base.Blknum, 0, opts.BlockCnt)
	var err error
	var ok bool

	// Clean the temporary files and makes sure block zero has been processed
	if ok, err := opts.Prepare(); !ok || err != nil {
		return err
	}

	runCount := uint64(0)
	// Loop until the user hits Cntl+C, until runCount runs out, or until
	// the server tells us to stop.
	for {
		// We create a new manager for each loop...we will populate it in a minute...
		bm := BlazeManager{
			chain: chain,
		}

		// Fetch the meta data which tells us how far along the index is.
		if bm.meta, err = opts.Conn.GetMetaData(testMode); err != nil {
			var ErrFetchingMeta = fmt.Errorf("error fetching meta data: %s", err)
			logger.Error(colors.BrightRed+ErrFetchingMeta.Error(), colors.Off)
			goto PAUSE
		}

		// This only happens if the chain and the index scraper are both started at the
		// same time (rarely). This protects against the case where the chain has no ripe blocks.
		// Report no error and sleep for a while.
		if bm.meta.ChainHeight() < base.Blknum(opts.Settings.UnripeDist) {
			goto PAUSE
		}

		// Another rare case, but here the user has reset his/her node but not removed
		// the index. In this case, the index is ahead of the chain. We go to sleep and
		// try again later in the hopes that the chain catches up.
		if !opts.DryRun && bm.meta.NextIndexHeight() > bm.meta.ChainHeight()+1 {
			var ErrIndexAhead = fmt.Errorf(
				"index (%d) is ahead of chain (%d)",
				bm.meta.NextIndexHeight(),
				bm.meta.ChainHeight(),
			)
			logger.Error(colors.BrightRed+ErrIndexAhead.Error(), colors.Off)
			goto PAUSE
		}

		// Let's start a new round...
		bm = BlazeManager{
			chain:        chain,
			opts:         opts,
			nRipe:        0,
			nUnripe:      0,
			timestamps:   make(map[base.Blknum]tslib.TimestampRecord, opts.BlockCnt),
			processedMap: make(map[base.Blknum]bool, opts.BlockCnt),
			meta:         bm.meta,
			nChannels:    int(opts.Settings.ChannelCount),
		}

		// Order dependant, be careful!
		// first block to scrape (one past end of previous round).
		bm.startBlock = bm.meta.NextIndexHeight()
		// if opts.StartBlock != 0 {
		// 	bm.startBlock = opts.StartBlock
		// }

		// user supplied, but not so many to pass the chain tip.
		bm.blockCount = base.Min(base.Blknum(opts.BlockCnt), bm.meta.ChainHeight()-bm.StartBlock()+1)
		// Unripe_dist behind the chain tip.
		bm.ripeBlock = bm.meta.ChainHeight() - base.Blknum(opts.Settings.UnripeDist)

		// These are the blocks we're going to process this round
		blocks = make([]base.Blknum, 0, bm.BlockCount())
		for block := bm.StartBlock(); block < bm.EndBlock(); block++ {
			blocks = append(blocks, block)
		}

		if len(blocks) == 0 {
			logger.Info("no blocks to scrape")
			goto PAUSE
		}

		if opts.Globals.Verbose {
			logger.Info("chain head:           ", bm.meta.ChainHeight())
			logger.Info("opts.BlockCnt:        ", opts.BlockCnt)
			logger.Info("ripe block:           ", bm.ripeBlock)
			logger.Info("perChunk:             ", bm.PerChunk())
			logger.Info("start block:          ", bm.StartBlock())
			logger.Info("block count:          ", bm.BlockCount())
			logger.Info("len(blocks):          ", len(blocks))
			if len(blocks) > 0 {
				logger.Info("blocks[0]:            ", blocks[0])
				logger.Info("blocks[len(blocks)-1]:", blocks[len(blocks)-1])
			}
		}

		// Scrape this round. Only quit on catostrophic errors. Report and sleep otherwise.
		if err, ok = bm.ScrapeBatch(blocks); !ok || err != nil {
			if err != nil {
				logger.Error(colors.BrightRed+err.Error(), colors.Off)
			}
			if !ok {
				break
			}
			goto PAUSE
		}

		if bm.nRipe == 0 {
			logger.Info(colors.Green+"no ripe files to consolidate", spaces, colors.Off)
			goto PAUSE

		} else {
			// Consilidate a chunk (if possible). Only quit on catostrophic errors. Report and sleep otherwise.
			if err, ok = bm.Consolidate(blocks); !ok || err != nil {
				if err != nil {
					logger.Error(colors.BrightRed+err.Error(), colors.Off)
				}
				if !ok {
					break
				}
				goto PAUSE
			}
		}

	PAUSE:
		runCount++
		if opts.RunCount != 0 && runCount >= opts.RunCount {
			// No reason to clean up here. Next round will do so and user can use these files in the meantime.
			logger.Info("run count reached")
			break
		}

		// sleep for a bit (there's no new blocks anyway if we're caught up).
		distanceFromHead := base.Blknum(28)
		if bm.meta != nil { // it may be nil if the node died
			distanceFromHead = bm.meta.ChainHeight() - bm.meta.StageHeight()
		}
		opts.pause(distanceFromHead)

		// defensive programming - just double checking our own understanding...
		count := file.NFilesInFolder(bm.RipeFolder())
		if count != 0 {
			_ = cleanEphemeralIndexFolders(chain)
			err := fmt.Errorf("%d unexpected ripe files in %s", count, bm.RipeFolder())
			logger.Error(colors.BrightRed+err.Error(), colors.Off)
		}

		// We want to clean up the unripe files. The chain may have (it frequently does)
		// re-orged. We want to re-qeury these next round. This is why we have an unripePath.
		if err = os.RemoveAll(bm.UnripeFolder()); err != nil {
			logger.Error(colors.BrightRed, err, colors.Off)
			return err
		}
	}

	// We've left the loop and we're done.
	return nil
}

var spaces = strings.Repeat(" ", 50)

// cleanEphemeralIndexFolders removes files in ripe and unripe
func cleanEphemeralIndexFolders(chain string) error {
	return file.CleanFolder(chain, config.PathToIndex(chain), []string{"ripe", "unripe"})
}
