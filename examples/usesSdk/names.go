package main

import (
	"bytes"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

// DoNames tests the When sdk function
func DoNames() {
	logger.Info("DoNames")

	opts := sdk.NamesOptions{
		Terms:     []string{"Giveth"},
		MatchCase: true,
	}

	buf := bytes.Buffer{}
	if err := opts.NamesBytes(&buf); err != nil {
		logger.Fatal(err)
	}

	file.StringToAsciiFile("usesSDK/names.json", buf.String())
	fmt.Println(buf.String())
}
