package scrapers

import (
	"fmt"
	"log"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/go-apps/blaze/utils"
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
				log.Print(utils.Yellow, "IndexScraper   ", utils.Blue, "[running --> paused]", utils.Off, ": ", IndexScraper.Counter, utils.Off, "\n")
			}
			IndexScraper.WasRunning = false
			time.Sleep(time.Duration(IndexScraper.Sleep) * time.Millisecond)

		} else {
			if !IndexScraper.WasRunning {
				log.Print(utils.Yellow, "IndexScraper   ", utils.Blue, "[paused --> running]", utils.Off, ": ", IndexScraper.Counter, utils.Off, "\n")
			}
			IndexScraper.WasRunning = true
			IndexScraper.Counter++
			log.Print(utils.Yellow, "IndexScraper   ", utils.Blue, "[sleep --> wake]", utils.Off, ": ", IndexScraper.Counter, utils.Off, "\n")
			for i := 0; i < 10; i++ {
				fmt.Println("I am here: ", i)
				time.Sleep(1 * time.Second)
				if !IndexScraper.Running {
					break
				}
			}
			log.Print(utils.Yellow, "IndexScraper   ", utils.Blue, "[wake --> sleep]", utils.Off, ": ", IndexScraper.Counter, utils.Off, "\n")
			time.Sleep(time.Duration(IndexScraper.Sleep) * time.Millisecond)
		}
	}
}
