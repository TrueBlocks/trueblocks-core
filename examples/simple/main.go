package main

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/sdk"
)

// main is a purposefully simple example that uses the TrueBlocks SDK. It retrieves blocks
// based on the options, and prints each block's number and date. It queries for
// monthly blocks using a block range (start-stop:step) of 0-19000000:monthly. If an
// error occurs the program exits. Note that this version of the code does not use
// the streaming version of the SDK. See the simpleStreaming example for that.
func main() {
	opts := sdk.WhenOptions{
		BlockIds: []string{"0-19000000:monthly"},
		Globals: sdk.Globals{
			Cache: true,
		},
	}

	// Query TrueBlocks core for the desired blocks (monthly blocks between 0 and 19000000)
	blocks, _, err := opts.When()
	if err != nil {
		fmt.Println(err)
		return
	}

	// Show the result
	for _, block := range blocks {
		fmt.Printf("%d\t%s\n", block.BlockNumber, block.Date())
	}
}
