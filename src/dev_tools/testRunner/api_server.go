package main

import (
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/sdk/v3"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

func getApiUrl() string {
	port := strings.ReplaceAll(os.Getenv("TB_TEST_API_SERVER"), ":", "")
	if port == "" {
		port = "8080"
	}
	return "localhost:" + port
}

func startApiServer() error {
	ready := make(chan bool)
	go sdk.NewDaemon(getApiUrl()).Start(ready)
	<-ready
	logger.Info(colors.Yellow + "Server started..." + colors.Off)
	return nil
}
