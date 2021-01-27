/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
package swagger

import (
	"net/http"
)

func AdminScrape(w http.ResponseWriter, r *http.Request) {
	callOneExtra(w, r, "chifra", "scrape")
}

func AdminStatus(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "cacheStatus")
}
