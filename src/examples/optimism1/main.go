package main

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/v0/sdk"
)

func main() {
	opts := sdk.WhenOptions{
		BlockIds: []string{"0x1", "0x2"},
	}
	blocks, _ := opts.When()
	fmt.Println("Hello world", blocks)
}
