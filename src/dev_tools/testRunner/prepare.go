package main

import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	sdk "github.com/TrueBlocks/trueblocks-sdk/v4"
)

func downloadAbis() error {
	logger.Info(colors.Yellow + "Downloading Abis..." + colors.Off)

	addrs := []string{
		"0xd9db270c1b5e3bd161e8c8503c55ceabee709552",
		"0x7d655c57f71464b6f83811c55d84009cd9f5221c",
		"0x45f783cce6b7ff23b2ab2d70e416cdb7d6055f51",
		"0x9ba00d6856a4edf4665bca2c2309936572473b7e",
		"0xdbd27635a534a3d3169ef0498beb56fb9c937489",
	}
	opts := sdk.AbisOptions{
		Addrs: addrs,
		Globals: sdk.Globals{
			Decache: true,
		},
	}
	os.Setenv("TB_TEST_MODE", "")
	logger.SetTestMode(false)
	if _, _, err := opts.Abis(); err != nil {
		logger.Fatal(err)
	}
	opts.Globals.Decache = false
	if _, _, err := opts.Abis(); err != nil {
		logger.Fatal(err)
	}
	logger.SetTestMode(true)
	os.Setenv("TB_TEST_MODE", "true")

	return nil
}
