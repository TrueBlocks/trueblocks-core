package main

import (
	"bytes"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/v0/sdk"
)

// DoSlurp tests the When sdk function
func DoSlurp() {
	logger.Info("DoSlurp")

	opts := sdk.SlurpOptions{
		Addrs:   []string{testAddrs[0]},
		PerPage: 10,
	}

	buf := bytes.Buffer{}
	if err := opts.Slurp(&buf); err != nil {
		logger.Fatal(err)
	}

	file.StringToAsciiFile("usesSDK/slurp.json", buf.String())
	fmt.Println(buf.String())
}
