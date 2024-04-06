// TODO: This can be auto generated
package main

import (
	"bytes"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/v0/sdk"
)

// DoStatus tests the When sdk function
func DoStatus() {
	logger.Info("DoStatus")

	opts := sdk.StatusOptions{
		Modes: sdk.SMBlocks,
	}

	buf := bytes.Buffer{}
	if err := opts.StatusBytes(&buf); err != nil {
		logger.Fatal(err)
	}

	file.StringToAsciiFile("usesSDK/status.json", buf.String())
	fmt.Println(buf.String())
}
