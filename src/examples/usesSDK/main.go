package main

import (
	"bytes"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
)

func main() {
	options := map[string]string{
		"addrs":      "trueblocks.eth",
		"logs":       "true",
		"articulate": "true",
		"cache":      "true",
	}

	var buf bytes.Buffer
	if err := sdk.Export(&buf, options); err != nil {
		logger.Fatal(err)
	}
	fmt.Println(buf.String())
}
