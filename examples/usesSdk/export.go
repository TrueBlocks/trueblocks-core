package main

import (
	"bytes"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

// DoExport tests the When sdk function
func DoExport() {
	logger.Info("DoExport")

	opts := sdk.ExportOptions{
		Addrs: []string{testAddrs[0]},
	}

	buf := bytes.Buffer{}
	if err := opts.ExportBytes(&buf); err != nil {
		logger.Fatal(err)
	}

	file.StringToAsciiFile("usesSDK/export.json", buf.String())
	fmt.Println(buf.String())
}
