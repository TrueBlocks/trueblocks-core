package main

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/sdk/v3"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/crud"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func autoName(args []string) {
	opts := sdk.NamesOptions{}
	address := base.HexToAddress(args[2])
	if names, _, err := opts.NamesAutoname(address); err != nil {
		printError(err.Error())
	} else {
		for _, name := range names {
			fmt.Printf("Name auto updated: %v\n", name.String())
		}
	}
}

func deleteName(args []string) {
	opts := sdk.NamesOptions{}
	address := base.HexToAddress(args[2])
	cd := crud.CrudFromName(types.Name{Address: address})
	if names, _, err := opts.ModifyName(crud.Delete, cd); err != nil {
		printError(err.Error())
	} else {
		for _, name := range names {
			fmt.Printf("Name deleted: %v\n", name.String())
		}
	}
}

func undeleteName(args []string) {
	opts := sdk.NamesOptions{}
	address := base.HexToAddress(args[2])
	cd := crud.CrudFromName(types.Name{Address: address})
	if names, _, err := opts.ModifyName(crud.Undelete, cd); err != nil {
		printError(err.Error())
	} else {
		for _, name := range names {
			fmt.Printf("Name deleted: %v\n", name.String())
		}
	}
}

func removeNode(args []string) {
	opts := sdk.NamesOptions{}
	address := base.HexToAddress(args[2])
	cd := crud.CrudFromName(types.Name{Address: address})
	if names, _, err := opts.ModifyName(crud.Remove, cd); err != nil {
		printError(err.Error())
	} else {
		for _, name := range names {
			fmt.Printf("Name deleted: %v\n", name.String())
		}
	}
}
