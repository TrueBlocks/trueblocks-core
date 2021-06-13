package server

/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/

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
