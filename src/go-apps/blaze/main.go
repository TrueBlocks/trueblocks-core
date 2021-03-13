package main

/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/

import (
	"log"
	"net/http"

	scrapers "github.com/TrueBlocks/trueblocks-core/src/go-apps/blaze/scrapers"
	tb "github.com/TrueBlocks/trueblocks-core/src/go-apps/blaze/server"
)

func main() {
	// Handle command line options
	err := tb.ParseOptions()
	if err != nil {
		log.Println("Could not parse command line.")
		return
	}

	// Let the user know what's going on...
	tb.StartupMessage()

	// Start listening on web sockets
	tb.RunWebsocketPool()

	// We always start the scrapers...the user will tell us to turn them on
	go scrapers.RunBlockScraper()
	go scrapers.RunMonitorScraper()

	// Start listening for requests
	log.Fatal(http.ListenAndServe(tb.Options.Port, tb.NewRouter()))
}
