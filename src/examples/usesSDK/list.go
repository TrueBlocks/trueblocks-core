// TODO: This can be auto generated
package main

import (
	"bytes"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/v0/sdk"
)

// DoList tests the When sdk function
func DoList() {
	logger.Info("DoList")

	opts := sdk.ListOptions{
		Addrs: []string{testAddrs[0]},
	}

	buf := bytes.Buffer{}
	if err := opts.ListBytes(&buf); err != nil {
		logger.Fatal(err)
	}

	file.StringToAsciiFile("usesSDK/list.json", buf.String())
	fmt.Println(buf.String())
}
