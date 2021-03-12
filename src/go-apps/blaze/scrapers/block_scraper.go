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

var BlockScraper Scraper

func RunBlockScraper() {
	for true {
		if (BlockScraper.Paused) {
			if (!BlockScraper.WasPaused) {
				log.Print(utils.Yellow, "BlockScraper is paused: ", BlockScraper.Counter, " ", Counter, utils.Off, "\n")
			}
			BlockScraper.WasPaused = true
			time.Sleep(1000 * time.Millisecond)
		} else {
			BlockScraper.Counter++
			Counter++
			log.Print(utils.Yellow, "BlockScrape awake", utils.Off, "\n")
			time.Sleep(1 * time.Second)
			log.Print(utils.Yellow, "BlockScrape sleeping: ", BlockScraper.Counter, " ", Counter, utils.Off, "\n")
			time.Sleep(1 * time.Second)
			BlockScraper.WasPaused = false
		}
	}
}
