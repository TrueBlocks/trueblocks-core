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

	tb "github.com/Great-Hill-Corporation/trueblocks-core/src/go-apps/goServer/cmd"
)

func main() {
	// handle some command line options
    port := flag.String("port", "8080", "the server's port")
    verbose := flag.Int("verbose", 0, "debug level")
	flag.Parse()

	// Assign them to a globally available options struct
	tb.Options.Verbose = *verbose
	tb.Options.Port = ":" + *port

	// Let the user know we're starting up...
	log.Printf("Starting TrueBlocks API server on port " + tb.Options.Port)
	out, err := exec.Command("cacheStatus", "--terse").Output()
	if err != nil {
		fmt.Printf("%s", err)
	} else {
		log.Printf(string(out[:]))
	}

	// Start listening on web sockets
	tb.RunWebsocketPool()

	// Start listening for requests
	log.Fatal(http.ListenAndServe(tb.Options.Port, tb.NewRouter()))
}
