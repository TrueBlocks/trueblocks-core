package main

import (
	"bytes"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

// DoConfig tests the When sdk function
func DoConfig() {
	logger.Info("DoConfig")

	opts := sdk.ConfigOptions{
		Mode: sdk.CMShow,
	}

	buf := bytes.Buffer{}
	if err := opts.ConfigBytes(&buf); err != nil {
		logger.Fatal(err)
	}

	file.StringToAsciiFile("usesSDK/config.json", buf.String())
	fmt.Println(buf.String())
}
