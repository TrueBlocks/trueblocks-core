package main

/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/

import (
	"flag"
	"fmt"
	"log"
	"net/http"
	"os/exec"
	"strings"

	tb "github.com/Great-Hill-Corporation/trueblocks-core/src/go-apps/goServer/cmd"
)

func main() {
	// handle some command line options
	flag.BoolVar(&tb.Options.Scrape, "scrape", false, "enable block scraper mode")
	flag.StringVar(&tb.Options.Port, "port", ":8080", "specify the server's port")
    flag.IntVar(&tb.Options.Verbose, "verbose", 0, "verbose level (between 0 and 10 inclusive)")
	flag.Parse()

	// Cleanup user input
	if !strings.HasPrefix(tb.Options.Port, ":") {
		tb.Options.Port = ":" + tb.Options.Port
	}

	// Let the user know we're starting up...
	log.Printf("Starting TrueBlocks API server on port " + tb.Options.Port)
	out, err := exec.Command("cacheStatus", "--terse").Output()
	if err != nil {
		fmt.Printf("%s", err)
	} else {
		log.Printf(string(out[:]))
	}
	if tb.Options.Scrape {
		log.Println("options.scrape: ", tb.Options.Scrape)
	}
	if tb.Options.Port != ":8080" {
		log.Println("options.port: ", tb.Options.Scrape)
	}
	if tb.Options.Verbose > 0 {
		log.Println("options.scrape: ", tb.Options.Scrape)
	}

	// Start listening on web sockets
	tb.RunWebsocketPool()

	// Start listening for requests
	log.Fatal(http.ListenAndServe(tb.Options.Port, tb.NewRouter()))
}
