package scrapers

/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/

var Counter uint64 = 0

type Scraper struct {
	Counter uint64
	Paused bool
	WasPaused bool
}
