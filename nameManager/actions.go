package main

import (
	"fmt"

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

// AutoName implements the chifra names --autoname <address> command.
func (a *App) AutoName(args []string) {
	opts := sdk.NamesOptions{
		Regular: a.IsRegular(),
	}
	address := base.HexToAddress(args[2])
	if result, _, err := opts.NamesAutoname(address); err != nil {
		printHelp(err)
	} else {
		if len(result) == 0 {
			printHelp(fmt.Errorf("%s is not an ERC20 token", address.Hex()))
		} else {
			for _, name := range result {
				fmt.Printf("Name auto updated: %v\n", name.String())
			}
		}
	}
}

// DeleteName implements the chifra names --delete <address> command.
func (a *App) DeleteName(args []string) {
	opts := sdk.NamesOptions{
		Regular: a.IsRegular(),
	}
	address := base.HexToAddress(args[2])
	cd := crud.CrudFromName(types.Name{Address: address})
	if result, _, err := opts.ModifyName(crud.Delete, cd); err != nil {
		printHelp(err)
	} else {
		for _, name := range result {
			fmt.Printf("Name deleted: %v\n", name.String())
		}
	}
}

// UndeleteName implements the chifra names --undelete <address> command.
func (a *App) UndeleteName(args []string) {
	opts := sdk.NamesOptions{
		Regular: a.IsRegular(),
	}
	address := base.HexToAddress(args[2])
	cd := crud.CrudFromName(types.Name{Address: address})
	if result, _, err := opts.ModifyName(crud.Undelete, cd); err != nil {
		printHelp(err)
	} else {
		for _, name := range result {
			fmt.Printf("Name undeleted: %v\n", name.String())
		}
	}
}

// RemoveName implements the chifra names --remove <address> command.
func (a *App) RemoveName(args []string) {
	opts := sdk.NamesOptions{
		Regular: a.IsRegular(),
	}
	address := base.HexToAddress(args[2])
	cd := crud.CrudFromName(types.Name{Address: address})
	if result, _, err := opts.ModifyName(crud.Remove, cd); err != nil {
		printHelp(err)
	} else {
		for _, name := range result {
			fmt.Printf("Name removed: %v\n", name.String())
		}
	}
}

// CleanNames implements the chifra names --clean command.
func (a *App) CleanNames() {
	opts := sdk.NamesOptions{
		Regular: a.IsRegular(),
	}
	if msgs, _, err := opts.NamesClean(); err != nil {
		printHelp(err)
	} else {
		for i, msg := range msgs {
			fmt.Println(i, msg.String())
		}
	}
}
