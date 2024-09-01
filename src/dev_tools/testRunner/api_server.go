package main

import (
	"fmt"
	"net"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/sdk/v3"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

var apiPort = ""

func getApiUrl() string {
	if apiPort != "" {
		return "localhost:" + apiPort
	}

	apiPort = strings.ReplaceAll(os.Getenv("TB_TEST_API_SERVER"), ":", "")
	if apiPort == "" {
		apiPort = fmt.Sprintf("%d", findAvailablePort())
	}
	return "localhost:" + apiPort
}

func startApiServer() error {
	ready := make(chan bool)
	go sdk.NewDaemon(getApiUrl()).Start(ready)
	<-ready
	logger.Info(colors.Yellow + "Server started..." + colors.Off)
	return nil
}

func findAvailablePort() int {
	listener, err := net.Listen("tcp", ":0")
	if err != nil {
		return 8080
	}
	defer listener.Close()
	addr := listener.Addr().(*net.TCPAddr)
	return addr.Port
}
