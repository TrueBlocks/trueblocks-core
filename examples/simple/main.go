package main

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/sdk"
)

func main() {
	opts := sdk.WhenOptions{
		BlockIds: []string{"0-19000000:monthly"},
		Globals: sdk.Globals{
			Cache: true,
		},
	}

	blocks, _, err := opts.When()
	if err != nil {
		fmt.Println(err)
		return
	}

	for _, block := range blocks {
		fmt.Printf("%d\t%s\n", block.BlockNumber, block.Date())
	}
}
