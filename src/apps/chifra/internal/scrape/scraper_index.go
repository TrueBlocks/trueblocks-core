package scrapePkg

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"sync"
)

func hasIndexerFlag(mode string) bool {
	return mode == "indexer" || mode == "both"
}

var IndexScraper Scraper

func RunIndexScraper(opts *ScrapeOptions, wg sync.WaitGroup, initialState bool) {
	defer wg.Done()

	var s *Scraper = &IndexScraper
	s.ChangeState(initialState)

	for {
		if !s.Running {
			if s.WasRunning {
				s.ShowStateChange("running", "paused")
			}
			s.WasRunning = false
			s.Pause()

		} else {
			if !s.WasRunning {
				s.ShowStateChange("paused", "running")
			}
			s.WasRunning = true
			s.Counter++
			s.ShowStateChange("sleep", "wake")

			/* -------------- */
			opts.Globals.PassItOn("blockScrape", opts.ToCmdLine())
			/* -------------- */

			s.ShowStateChange("wake", "sleep")
			if s.Running {
				s.Pause()
			}
		}
	}
}
