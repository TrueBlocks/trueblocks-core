package scrapePkg

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
)

func hasIndexerFlag(mode string) bool {
	return mode == "indexer" || mode == "both"
}

var IndexScraper Scraper

func (opts *ScrapeOptions) RunIndexScraper(wg *sync.WaitGroup) {
	defer wg.Done()

	var s *Scraper = &IndexScraper
	s.ChangeState(true)

	for {
		if !s.Running {
			s.Pause()

		} else {
			opts.Globals.PassItOn("blockScrape", opts.Globals.Chain, opts.ToCmdLine(), opts.Globals.ToCmdLine())
			if s.Running {
				// We sleep under two conditions
				//   1) the user has told us an explicit amount of time to Sleep
				//   2) we're close enough to the head that we should sleep because there
				//      are no new blocks (UnripeDist defaults to 28 blocks)
				//
				// If we're closeEnough and the user specified a sleep value less than
				// 14 seconds, there's not reason to not sleep
				meta, _ := rpcClient.GetMetaData(opts.Globals.Chain, false)
				distanceFromHead := meta.Latest - meta.Staging
				closeEnough := distanceFromHead <= (2 * opts.UnripeDist)
				// TODO: per chain data
				if closeEnough {
					opts.Sleep = 13
				}
				isDefault := opts.Sleep == 14 || opts.Sleep == 13
				if !isDefault || closeEnough {
					if closeEnough {
						logger.Log(logger.Info, "Close enough to head. Sleeping for", opts.Sleep, "seconds.")
					} else {
						logger.Log(logger.Info, "Sleeping for", opts.Sleep, "seconds.")
					}
					s.Pause()
				}
			}
		}
	}
}
