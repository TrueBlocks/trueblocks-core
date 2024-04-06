// TODO: This can be auto generated
package main

import (
	"bytes"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/v0/sdk"
)

// DoAbis tests the When sdk function
func DoAbis() {
	logger.Info("DoAbis")

	opts := sdk.AbisOptions{
		Addrs: []string{"uniswap.eth"},
	}

	buf := bytes.Buffer{}
	if err := opts.AbisBytes(&buf); err != nil {
		logger.Fatal(err)
	}

	file.StringToAsciiFile("usesSDK/abis.json", buf.String())
	fmt.Println(buf.String())
}
