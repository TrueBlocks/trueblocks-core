package main

/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/

import (
	"log"
	"net/http"
	"time"

	tb "github.com/TrueBlocks/trueblocks-core/src/go-apps/blaze/cmd"
	utils "github.com/TrueBlocks/trueblocks-core/src/go-apps/blaze/utils"
	scrap "github.com/TrueBlokcs/trueblocks-core/src/go-apps/blaze/scrapers"
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

	if tb.Options.BlockScrape {
		go func() {
			for true {
				scrap.BlockScraper.Counter++
				log.Print(utils.Yellow, "BlockScrape awake", utils.Off, "\n")
				time.Sleep(1 * time.Second)
				log.Print(utils.Yellow, "BlockScrape sleeping: ", scrap.BlockScraper.Counter, utils.Off, "\n")
				time.Sleep(1 * time.Second)
			}
		}()
	}

	if tb.Options.MonitorScrape {
		go func() {
			for true {
				scrap.MonitorScraper.Counter++
				log.Print(utils.Blue, "MonitorScrape awake", utils.Off, "\n")
				time.Sleep(1 * time.Second)
				log.Print(utils.Blue, "MonitorScrape sleeping: ", scrap.MonitorScraper.Counter, utils.Off, "\n")
				time.Sleep(14 * time.Second)
			}
		}()
	}

	// Start listening for requests
	log.Fatal(http.ListenAndServe(tb.Options.Port, tb.NewRouter()))
}
