package server

/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/

import (
	"net/http"

	scrapers "github.com/TrueBlocks/trueblocks-core/src/go-apps/blaze/scrapers"
)

// ManageScraper handles scraper commands
func ManageScraper(w http.ResponseWriter, r *http.Request) {
	scrapers.IndexScraper.Running = !scrapers.IndexScraper.Running
	scrapers.MonitorScraper.Running = !scrapers.MonitorScraper.Running
}
