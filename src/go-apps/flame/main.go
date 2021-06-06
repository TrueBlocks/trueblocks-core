package main

/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/

import (
	"log"
	"net/http"

	scrapers "github.com/TrueBlocks/trueblocks-core/src/go-apps/blaze/scrapers"
	server "github.com/TrueBlocks/trueblocks-core/src/go-apps/blaze/server"
)

func main() {
	// Handle command line options
	err := server.ParseOptions()
	if err != nil {
		log.Println("Could not parse command line.")
		return
	}

	// Start listening on web sockets
	server.RunWebsocketPool()

	// We always start the scrapers...the user will tell us to turn them on
	go scrapers.RunIndexScraper()
	go scrapers.RunMonitorScraper()

	// Start listening for requests
	log.Fatal(http.ListenAndServe(server.Options.Port, server.NewRouter()))
}
