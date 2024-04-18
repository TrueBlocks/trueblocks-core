package main

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/v0/sdk"
)

func downloadAbis() {
	logger.Info(colors.Yellow + "Downloading Abis..." + colors.Off)

	addrs := []string{
		"0xd9db270c1b5e3bd161e8c8503c55ceabee709552",
		"0x7d655c57f71464b6f83811c55d84009cd9f5221c",
	}
	opts := sdk.AbisOptions{
		Addrs: addrs,
		Globals: sdk.Globals{
			Decache: true,
		},
	}
	if _, _, err := opts.Abis(); err != nil {
		logger.Fatal(err)
	}

	opts.Globals.Decache = false
	if _, _, err := opts.Abis(); err != nil {
		logger.Fatal(err)
	}
}
