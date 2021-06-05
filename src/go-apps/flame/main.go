package main

/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/

import (
	"flag"
	"fmt"
	"log"
	"net/http"
	"os"
	"os/exec"
	"runtime"
	"strings"

	tb "github.com/TrueBlocks/trueblocks-core/src/go-apps/goServer/cmd"
	homedir "github.com/mitchellh/go-homedir"
)

func main() {
	// handle some command line options
	flag.StringVar(&tb.Options.Port, "port", ":8080", "specify the server's port")
    flag.IntVar(&tb.Options.Verbose, "verbose", 0, "set verbose level (optional level defaults to 1)")
	flag.Parse()

	// Cleanup user input
	if !strings.HasPrefix(tb.Options.Port, ":") {
		tb.Options.Port = ":" + tb.Options.Port
	}

	// Let the user know we're starting up...
	log.Printf("Starting TrueBlocks API server on port " + tb.Options.Port)
	out, err := exec.Command("cacheStatus", "--terse").Output()
	if err != nil {
		fmt.Printf("There was an error with the RPC server: %s", err)

	} else if len(out) == 0 {
		// The results of the call to cacheStatus went to stderr. We assume here
		// that this happened because the RPC server is not running or unavailable.
		home, err := homedir.Dir()
		if err != nil {
			fmt.Println(err)
			os.Exit(1)
		}
		configPath := "<unset>"
		if (runtime.GOOS == "darwin") {
			configPath = home + "/Library/Application Support/TrueBlocks"
		} else if (runtime.GOOS == "linux") {
			configPath = home + "/.local/share/trueblocks"
		} else {
			fmt.Println("Windows not supported.")
		}
  		log.Printf("The Ethereum RPC server was not found. To correct the problem, edit rpcProvider")
  		log.Printf("in the file " + configPath)

	} else {
		log.Printf(string(out[:]))

		// Start listening on web sockets
		tb.RunWebsocketPool()

		// Start listening for requests
		log.Fatal(http.ListenAndServe(tb.Options.Port, tb.NewRouter()))
	}
}
