// TODO: This can be auto generated
package main

import (
	"bytes"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/v0/sdk"
)

// DoWhen tests the When sdk function
func DoWhen() {
	logger.Info("DoWhen")

	opts := sdk.WhenOptions{
		Globals: sdk.Globals{
			Verbose: true,
			Cache:   true,
		},
		BlockIds: testBlocks,
	}

	buf := bytes.Buffer{}
	if err := opts.WhenBytes(&buf); err != nil {
		logger.Fatal(err)
	}

	file.StringToAsciiFile("usesSDK/when.json", buf.String())
	fmt.Println(buf.String())
}
