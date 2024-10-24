package main

import (
	"fmt"
	"os"
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/crud"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/usage"
	sdk "github.com/TrueBlocks/trueblocks-sdk/v3"
)

func main() {
	app := ParseArgs()

	if app.database == names.DatabaseRegular {
		if !usage.QueryUser("Are you sure you want to operate against the Regular names database (Y/n)?", "Quitting...") {
			os.Exit(0)
		}
	}

	if app.dryrun {
		backupAllFiles()
		defer restoreAllFiles()
	}

	switch app.action {
	case AutoName:
		app.AutoName(os.Args)
		return
	case Delete:
		app.DeleteName(os.Args)
		return
	case Undelete:
		app.UndeleteName(os.Args)
		return
	case Remove:
		app.RemoveName(os.Args)
		return
	case Clean:
		app.CleanNames()
		return
	case Publish:
		app.PublishNames()
		return
	}

	var name types.Name

	name.Address, _, _ = getValue(1, "address", "0xf503017d7baf7fbc0fff7492b751025c6a78179b")
	_, name.Name, _ = getValue(2, "name", "TrueBlocks 2020 Wallet 0xf503")
	_, name.Tags, _ = getValue(3, "tags", defTags)
	_, name.Source, _ = getValue(4, "source", defSource)
	_, name.Symbol, _ = getValue(5, "symbol", "")
	_, _, name.Decimals = getValue(6, "decimals", "")

	opts := sdk.NamesOptions{
		Regular: app.IsRegular(),
	}
	cd := crud.CrudFromName(name)
	if names, _, err := opts.ModifyName(crud.Update, cd); err != nil {
		printHelp(err)

	} else {
		for _, name := range names {
			fmt.Printf("Names: %v\n", name.String())
		}
	}
}

func getFilenames() []string {
	return []string{
		filepath.Join(config.MustGetPathToChainConfig("mainnet"), string(names.DatabaseRegular)),
		filepath.Join(config.MustGetPathToChainConfig("mainnet"), string(names.DatabaseCustom)),
	}
}

func backupAllFiles() {
	backupFile := func(fileName string) {
		backupName := fileName + ".bak"
		_, _ = file.Copy(backupName, fileName)
	}
	for _, file := range getFilenames() {
		logger.Info("Dryrun - backing up file:", file)
		backupFile(file)
	}
}

func restoreAllFiles() {
	restoreFile := func(fileName string) {
		backupName := fileName + ".bak"
		_, _ = file.Copy(fileName, backupName)
		_ = os.Remove(backupName)
	}
	for _, file := range getFilenames() {
		logger.Info("Dryrun - restoring file:", file)
		restoreFile(file)
	}
}
