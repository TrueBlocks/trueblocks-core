package scrapers

import (
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

func RunBlockScraper() {
	for true {
		if !IndexScraper.Running {
			if IndexScraper.WasRunning {
				log.Print(utils.Yellow, "IndexScraper is paused: ", IndexScraper.Counter, " ", Counter, utils.Off, "\n")
			}
			IndexScraper.WasRunning = false
			time.Sleep(1000 * time.Millisecond)
		} else {
			IndexScraper.WasRunning = true
			IndexScraper.Counter++
			Counter++
			log.Print(utils.Green, "IndexScraper awake", utils.Off, "\n")
			time.Sleep(1 * time.Second)
			log.Print(utils.Yellow, "IndexScraper sleeping: ", IndexScraper.Counter, " ", Counter, utils.Off, "\n")
			time.Sleep(1 * time.Second)
		}
	}
}
