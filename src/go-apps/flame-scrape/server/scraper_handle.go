package server

/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/

import (
	"fmt"
	"log"
	"net/http"

	utils "github.com/TrueBlocks/trueblocks-core/src/go-apps/blaze/utils"
)

// ManageScraper handles scraper commands
func ManageScraper(w http.ResponseWriter, r *http.Request) {
	status, exists := utils.GetParam("status", "both", r)
	if !exists {
		toggle, ok := utils.GetParam("toggle", "both", r)
		if !ok {
			log.Println("'toggle' param is missing")
			return
		}
		mode, ok := utils.GetParam("mode", "", r)
		if !ok {
			log.Println("'mode' param is missing")
			return
		}

		if toggle == "indexer" || toggle == "both" {
			IndexScraper.ChangeState(mode == "true")
		}
		if toggle == "monitors" || toggle == "both" {
			MonitorScraper.ChangeState(mode == "true")
		}
		scraperStatus("toggle", w, r)

	} else {
		log.Println("status: ", status)
		scraperStatus("status", w, r)
	}
}

// scraperStatus presents the result
func scraperStatus(msg string, w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "application/json; charset=UTF-8")
	w.Header().Set("Access-Control-Allow-Origin", "*")
	w.Header().Set("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept")
	w.Header().Set("Access-Control-Allow-Methods", "PUT, POST, GET, DELETE, OPTIONS")
	w.WriteHeader(http.StatusOK)
	fmt.Fprint(w, "{ ")
	fmt.Fprint(w, "\"msg\": \"")
	fmt.Fprint(w, msg)
	fmt.Fprint(w, "\", ")
	fmt.Fprint(w, "\"indexer\": ")
	fmt.Fprint(w, IndexScraper.ToJson())
	fmt.Fprint(w, ", ")
	fmt.Fprint(w, "\"monitor\": ")
	fmt.Fprint(w, MonitorScraper.ToJson())
	fmt.Fprint(w, " }")
}
