// TODO: This can be auto generated
package main

import (
	"bytes"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/v0/sdk"
)

// DoBlocks tests the Blocks sdk function
func DoBlocks() {
	logger.Info("DoMonitors")

	opts := sdk.BlocksOptions{
		BlockIds: testBlocks,
		Hashes:   true,
	}

	var buf bytes.Buffer
	if err := opts.BlocksBytes(&buf); err != nil {
		logger.Fatal(err)
	}

	file.StringToAsciiFile("usesSDK/blocks.json", buf.String())
	fmt.Println(buf.String())
}
