package main

import (
	"bytes"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/v0/sdk"
)

// DoNames tests the When sdk function
func DoNames() {
	logger.Info("DoNames")

	opts := sdk.NamesOptions{
		Terms:     []string{"Giveth"},
		MatchCase: true,
	}

	buf := bytes.Buffer{}
	if err := opts.Names(&buf); err != nil {
		logger.Fatal(err)
	}

	file.StringToAsciiFile("usesSDK/names.json", buf.String())
	fmt.Println(buf.String())
}
