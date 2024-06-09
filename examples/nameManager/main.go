package main

import (
	"fmt"
	"os"

	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func main() {
	if ok, action := parseArgs(); !ok {
		return
	} else {
		switch action {
		case "autoname":
			autoName(os.Args)
			return
		case "delete":
			deleteName(os.Args)
			return
		case "undelete":
			undeleteName(os.Args)
			return
		case "remove":
			removeNode(os.Args)
			return
		}
	}

	var name types.Name

	name.Address, _, _ = getValue(1, "address", "0xf503017d7baf7fbc0fff7492b751025c6a78179b")
	_, name.Name, _ = getValue(2, "name", "TrueBlocks 2020 Wallet 0xf503")
	_, name.Tags, _ = getValue(3, "tags", defTags)
	_, name.Source, _ = getValue(4, "source", defSource)
	_, name.Symbol, _ = getValue(5, "symbol", "")
	_, _, name.Decimals = getValue(6, "decimals", "")

	fmt.Printf("Address: %s\n", name.Address)
	os.Setenv("TB_NAME_ADDRESS", name.Address.Hex())

	fmt.Printf("Name: %s\n", name.Name)
	os.Setenv("TB_NAME_NAME", name.Name)

	fmt.Printf("TAGS: %s\n", name.Tags)
	os.Setenv("TB_NAME_TAGS", name.Tags)

	fmt.Printf("SOURCE: %s\n", name.Source)
	os.Setenv("TB_NAME_SOURCE", name.Source)

	fmt.Printf("DECIMALS: %d\n", name.Decimals)
	os.Setenv("TB_NAME_SYMBOL", name.Symbol)

	fmt.Printf("SYMBOL: %s\n", name.Symbol)
	os.Setenv("TB_NAME_DECIMALS", fmt.Sprintf("%d", name.Decimals))

	opts := sdk.NamesOptions{}
	opts.Terms = []string{name.Address.Hex()}
	opts.Custom = true
	if names, _, err := opts.NamesCreate(); err != nil {
		printError(err.Error())
	} else {
		for _, name := range names {
			fmt.Printf("Names: %v\n", name.String())
		}
	}
}
