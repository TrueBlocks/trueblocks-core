package main

import (
	"flag"
	"fmt"
	"log"
	"net/http"
	"os/exec"

	tb "github.com/Great-Hill-Corporation/trueblocks-core/src/go-apps/goServer/cmd"
)

func main() {
    port := flag.String("port", "8080", "the server's port")
    verbose := flag.Int("verbose", 0, "debug level")
	flag.Parse()
	tb.Options.Verbose = *verbose
	tb.Options.Port = ":" + *port

	// Let the user know what's going on...
	log.Printf("Starting TrueBlocks API server on port " + tb.Options.Port)
	out, err := exec.Command("cacheStatus", "--terse").Output()
	if err != nil {
		fmt.Printf("%s", err)
	} else {
		output := string(out[:])
		log.Printf(output)
	}

	// Enable web sockets
	tb.RunWebsocketPool()

	// Start listening
	log.Fatal(http.ListenAndServe(tb.Options.Port, tb.NewRouter()))
}
