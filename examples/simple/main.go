package main

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/sdk"
)

func main() {
	opts := sdk.WhenOptions{
		BlockIds: []string{"18000000-19000000:daily"},
		Globals: sdk.Globals{
			Cache: true,
		},
	}
	blocks, _, _ := opts.When()
	for _, block := range blocks {
		fmt.Printf("%d\t%s\n", block.BlockNumber, block.Date())
	}
}
