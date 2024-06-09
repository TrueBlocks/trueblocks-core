package main

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

func autoName(args []string) {
	opts := sdk.NamesOptions{}
	names, _, err := opts.NamesAutoname(base.HexToAddress(args[2]))
	if err != nil {
		printError(err.Error())
		return
	}
	for _, name := range names {
		fmt.Printf("Name auto updated: %v\n", name.String())
	}
}

func deleteName(args []string) {
	opts := sdk.NamesOptions{}
	opts.Terms = args[2:]
	names, _, err := opts.NamesDelete()
	if err != nil {
		printError(err.Error())
		return
	}
	for _, name := range names {
		fmt.Printf("Name deleted: %v\n", name.String())
	}
}

func undeleteName(args []string) {
	opts := sdk.NamesOptions{}
	opts.Terms = args[2:]
	names, _, err := opts.NamesUndelete()
	if err != nil {
		printError(err.Error())
		return
	}
	for _, name := range names {
		fmt.Printf("Name undeleted: %v\n", name.String())
	}
}

func removeNode(args []string) {
	opts := sdk.NamesOptions{}
	opts.Terms = args[2:]
	names, _, err := opts.NamesRemove()
	if err != nil {
		printError(err.Error())
		return
	}
	for _, name := range names {
		fmt.Printf("Name removed: %v\n", name.String())
	}
}
