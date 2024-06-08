package main

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/sdk"
)

func main() {
	opts := sdk.NamesOptions{}
	opts.Terms = []string{"0x000"}
	names, _, _ := opts.Names()
	for _, name := range names {
		fmt.Println(name.String())
	}
}
