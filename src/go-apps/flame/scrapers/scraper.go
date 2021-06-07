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
	Color      string
	Name       string
}

func (scraper *Scraper) ShowStateChange(from, to string) {
	log.Print(scraper.Color, scraper.Name, " ", utils.Blue, "[", from, " --> ", to, "]", utils.Off, ": ", scraper.Counter, "\n")
}
