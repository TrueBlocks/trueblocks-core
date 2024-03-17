package main

import (
	"bytes"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/v0/sdk"
)

// DoState tests the state sdk function
func DoState() {
	logger.Info("DoState")

	opts := sdk.StateOptions{
		Globals: sdk.Globals{
			Verbose: true,
			Cache:   true,
		},
		BlockIds: testBlocks,
		Addrs:    testAddrs,
	}

	buf := bytes.Buffer{}
	if err := opts.State(&buf); err != nil {
		logger.Fatal(err)
	}

	file.StringToAsciiFile("usesSDK/state.json", buf.String())
	fmt.Println(buf.String())
}
