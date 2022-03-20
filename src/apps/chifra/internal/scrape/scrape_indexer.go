package scrapePkg

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
)

func hasIndexerFlag(mode string) bool {
	return mode == "indexer" || mode == "both"
}

var IndexScraper Scraper

func (opts *ScrapeOptions) RunIndexScraper(wg *sync.WaitGroup, initialState bool) {
	defer wg.Done()

	var s *Scraper = &IndexScraper
	s.ChangeState(initialState)

	for {
		if !s.Running {
			s.Pause()

		} else {
			opts.Globals.PassItOn("blockScrape", opts.ToCmdLine())
			if s.Running {
				if rpcClient.DistanceFromHead(opts.Globals.Chain) <= (2 * opts.UnripeDist) {
					s.Pause()
				}
			}
		}
	}
}
