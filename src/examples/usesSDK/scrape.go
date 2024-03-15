package main

import (
	"bytes"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/v0/sdk"
)

// DoScrape tests the When sdk function
func DoScrape() {
	logger.Info("DoScrape")

	opts := sdk.ScrapeOptions{}

	buf := bytes.Buffer{}
	if err := opts.Scrape(&buf); err != nil {
		logger.Fatal(err)
	}

	file.StringToAsciiFile("usesSDK/scrape.json", buf.String())
	fmt.Println(buf.String())
}
