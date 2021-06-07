package scrapers

import (
	"fmt"
	"time"
)

/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/

var IndexScraper Scraper

func RunIndexScraper() {
	for true {
		if !IndexScraper.Running {
			if IndexScraper.WasRunning {
				IndexScraper.ShowStateChange("running", "paused")
			}
			IndexScraper.WasRunning = false
			time.Sleep(time.Duration(IndexScraper.Sleep) * time.Millisecond)
		} else {
			if !IndexScraper.WasRunning {
				IndexScraper.ShowStateChange("paused", "running")
			}
			IndexScraper.WasRunning = true
			IndexScraper.Counter++
			IndexScraper.ShowStateChange("sleep", "wake")
			for i := 0; i < 10; i++ {
				fmt.Println("I am here: ", i)
				time.Sleep(1 * time.Second)
				if !IndexScraper.Running {
					break
				}
			}
			IndexScraper.ShowStateChange("wake", "sleep")
			if IndexScraper.Running {
				time.Sleep(time.Duration(IndexScraper.Sleep) * time.Millisecond)
			}
		}
	}
}
