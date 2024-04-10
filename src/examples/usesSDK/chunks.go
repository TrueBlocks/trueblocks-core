package main

import (
	"bytes"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/v0/sdk"
)

// DoChunks tests the When sdk function
func DoChunks() {
	logger.Info("DoChunks")

	opts := sdk.ChunksOptions{
		Mode: sdk.CMIndex,
	}

	buf := bytes.Buffer{}
	if err := opts.ChunksBytes(&buf); err != nil {
		logger.Fatal(err)
	}

	file.StringToAsciiFile("usesSDK/chunks.json", buf.String())
	fmt.Println(buf.String())
}
