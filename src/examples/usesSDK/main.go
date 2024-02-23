package main

import (
	"bytes"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/sdk/v1"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

// DoBlocks tests the Blocks sdk function
func DoBlocks() {
	var buf bytes.Buffer
	if err := sdk.Blocks(&buf, sdk.BlocksOptions{
		Blocks: testBlocks,
		Hashes: true,
	}); err != nil {
		logger.Fatal(err)
	}

	fmt.Println(buf.String())
}

// DoWhen tests the When sdk function
func DoWhen() {
	var buf bytes.Buffer
	if err := sdk.When(&buf, sdk.WhenOptions{
		Blocks:     testBlocks,
		Timestamps: true,
		Globals:    sdk.Globals{Fmt: sdk.Csv, NoHeader: true},
	}); err != nil {
		logger.Fatal(err)
	}

	fmt.Println(buf.String())
}

func main() {
	DoBlocks()
	DoWhen()
}

var testBlocks = []base.Blknum{1, 1001001, 15000001}
