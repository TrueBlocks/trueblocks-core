package main

import (
	"bytes"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/v0/sdk"
)

// DoBlocks tests the Blocks sdk function
func DoBlocks() {
	var buf bytes.Buffer

	opts := sdk.BlocksOptions{
		BlockIds: testBlocks,
		Hashes:   true,
	}

	if err := opts.Blocks(&buf); err != nil {
		logger.Fatal(err)
	}

	fmt.Println(buf.String())
}

// DoWhen tests the When sdk function
func DoWhen() {
	opts := sdk.WhenOptions{
		Globals: sdk.Globals{
			Fmt:     sdk.Csv,
			Verbose: true,
		},
		BlockIds: testBlocks,
		// Timestamps: true,
	}

	buf := bytes.Buffer{}
	if err := opts.When(&buf); err != nil {
		logger.Fatal(err)
	}

	fmt.Println(buf.String())
}

func main() {
	DoBlocks()
	DoWhen()
}

var testBlocks = []string{
	"13800-1000000:monthly",
	"14011011",
}
