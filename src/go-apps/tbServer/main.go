package main

import (
	"fmt"
	"log"
	"net/http"
	"os/exec"

	tb "github.com/Great-Hill-Corporation/trueblocks-core/src/go-apps/goServer/cmd"
)

func main() {
	log.Printf("Starting TrueBlocks API server on port :8080")
	out, err := exec.Command("cacheStatus", "--terse").Output()
	if err != nil {
		fmt.Printf("%s", err)
	} else {
		output := string(out[:])
		log.Printf(output)
	}
	tb.RunWebsocketPool()
	log.Fatal(http.ListenAndServe(":8080", tb.NewRouter()))
}
