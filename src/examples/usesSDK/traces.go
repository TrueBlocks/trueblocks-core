package main

import (
	"bytes"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/v0/sdk"
)

// DoTraces tests the When sdk function
func DoTraces() {
	logger.Info("DoTraces")

	opts := sdk.TracesOptions{
		TransactionIds: []string{"10001002.0"},
	}

	buf := bytes.Buffer{}
	if err := opts.Traces(&buf); err != nil {
		logger.Fatal(err)
	}

	file.StringToAsciiFile("usesSDK/traces.json", buf.String())
	fmt.Println(buf.String())
}
