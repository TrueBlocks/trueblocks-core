package main

import (
	"fmt"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	sdk "github.com/TrueBlocks/trueblocks-sdk/v4"
)

func startApiServer() (apiUrl string, err error) {
	apiUrl = getApiUrl()
	ready := make(chan bool)
	go sdk.NewDaemon(apiUrl).Start(ready)
	<-ready
	return
}

// apiPort is a global variable to store the port of the API server
// of which there can be only one.
var apiPort = ""

func getApiUrl() string {
	// use the same port if we've already started the server...
	if apiPort != "" {
		return "localhost:" + apiPort
	}

	apiPort = strings.ReplaceAll(os.Getenv("TB_API_PORT"), ":", "")
	if apiPort == "" {
		apiPort = fmt.Sprintf("%d", rpc.FindAvailablePort())
	}

	return "localhost:" + apiPort
}
