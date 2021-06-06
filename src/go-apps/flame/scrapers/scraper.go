package scrapers

import (
	"log"

	"github.com/TrueBlocks/trueblocks-core/src/go-apps/blaze/utils"
)

/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/

type Scraper struct {
	Counter    uint64
	Running    bool
	WasRunning bool
	Sleep      int64
}

func (scraper *Scraper) ChangeState(from, to string) {
	log.Print(utils.Purple, "MonitorScraper ", utils.Blue, "[running --> paused]", utils.Off, ": ", scraper.Counter, utils.Off, "\n")
}
