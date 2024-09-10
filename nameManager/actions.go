package main

import (
	"fmt"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/crud"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	sdk "github.com/TrueBlocks/trueblocks-sdk/v3"
)

type Action string

const (
	AutoName Action = "autoname"
	Delete   Action = "delete"
	Undelete Action = "undelete"
	Remove   Action = "remove"
	Clean    Action = "clean"
	Publish  Action = "publish"
)

func autoName(args []string) {
	opts := sdk.NamesOptions{
		Regular: os.Getenv("TB_NAMEMANAGER_REGULAR") == "true",
	}
	address := base.HexToAddress(args[2])
	if result, _, err := opts.NamesAutoname(address); err != nil {
		printError(err.Error())
	} else {
		sortNames()
		for _, name := range result {
			fmt.Printf("Name auto updated: %v\n", name.String())
		}
	}
}

func deleteName(args []string) {
	opts := sdk.NamesOptions{
		Regular: os.Getenv("TB_NAMEMANAGER_REGULAR") == "true",
	}
	address := base.HexToAddress(args[2])
	cd := crud.CrudFromName(types.Name{Address: address})
	if result, _, err := opts.ModifyName(crud.Delete, cd); err != nil {
		printError(err.Error())
	} else {
		sortNames()
		for _, name := range result {
			fmt.Printf("Name deleted: %v\n", name.String())
		}
	}
}

func undeleteName(args []string) {
	opts := sdk.NamesOptions{
		Regular: os.Getenv("TB_NAMEMANAGER_REGULAR") == "true",
	}
	address := base.HexToAddress(args[2])
	cd := crud.CrudFromName(types.Name{Address: address})
	if result, _, err := opts.ModifyName(crud.Undelete, cd); err != nil {
		printError(err.Error())
	} else {
		sortNames()
		for _, name := range result {
			fmt.Printf("Name undeleted: %v\n", name.String())
		}
	}
}

func removeNode(args []string) {
	opts := sdk.NamesOptions{
		Regular: os.Getenv("TB_NAMEMANAGER_REGULAR") == "true",
	}
	address := base.HexToAddress(args[2])
	cd := crud.CrudFromName(types.Name{Address: address})
	if result, _, err := opts.ModifyName(crud.Remove, cd); err != nil {
		printError(err.Error())
	} else {
		sortNames()
		for _, name := range result {
			fmt.Printf("Name removed: %v\n", name.String())
		}
	}
}

func cleanNames() {
	opts := sdk.NamesOptions{
		Regular: os.Getenv("TB_NAMEMANAGER_REGULAR") == "true",
	}
	if msgs, _, err := opts.NamesClean(); err != nil {
		printError(err.Error())
	} else {
		for i, msg := range msgs {
			fmt.Println(i, msg.String())
		}
	}
}

func sortNames() {
	// fn :=
	// opts := sdk.NamesOptions{
	// 	Regular: os.Getenv("TB_NAMEMANAGER_REGULAR") == "true",
	// 	Custom:  os.Getenv("TB_NAMEMANAGER_REGULAR") != "true",
	// 	DryRun:  true,
	// }
	// if names, _, err := opts.Names(); err != nil {
	// 	printError(err.Error())
	// } else {
	// 	for _, name := range names {
	// 		fmt.Println(name)
	// 	}
	// }
}
