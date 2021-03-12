package scrapers

/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/

import (
	"log"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/go-apps/blaze/utils"
)

var MonitorScraper Scraper

func RunMonitorScraper() {
	for true {
		if (MonitorScraper.Paused) {
			if (!MonitorScraper.WasPaused) {
				log.Print(utils.Blue, "MonitorScrape is paused: ", MonitorScraper.Counter, " ", Counter, utils.Off, "\n")
			}
			MonitorScraper.WasPaused = true
			time.Sleep(1000 * time.Millisecond)
		} else {
			MonitorScraper.Counter++
			Counter += 100
			log.Print(utils.Blue, "MonitorScrape awake", utils.Off, "\n")
			time.Sleep(200 * time.Millisecond)
			log.Print(utils.Blue, "MonitorScrape sleeping: ", MonitorScraper.Counter, " ", Counter, utils.Off, "\n")
			time.Sleep(300 * time.Millisecond)
			MonitorScraper.WasPaused = false
		}
	}
}
