package main

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	sdk "github.com/TrueBlocks/trueblocks-sdk/v5"
)

func downloadAbis() error {
	logger.SetTestMode(false)
	defer logger.SetTestMode(true)

	logger.Info(colors.Yellow + "Downloading Abis..." + colors.Off)

	addrs := []string{
		"0xd9db270c1b5e3bd161e8c8503c55ceabee709552",
		"0x7d655c57f71464b6f83811c55d84009cd9f5221c",
		"0x45f783cce6b7ff23b2ab2d70e416cdb7d6055f51",
		"0x9ba00d6856a4edf4665bca2c2309936572473b7e",
		"0xdbd27635a534a3d3169ef0498beb56fb9c937489",
		"0xbb9bc244d798123fde783fcc1c72d3bb8c189413",
	}

	for _, addr := range addrs {
		opts := sdk.AbisOptions{
			Addrs: []string{addr},
			Globals: sdk.Globals{
				Decache: true,
			},
		}
		if _, _, err := opts.Abis(); err != nil {
			logger.Fatal(err)
		}
		opts.Globals.Decache = false
		if abis, _, err := opts.Abis(); err != nil {
			logger.Fatal(err)
		} else {
			logger.Info(colors.Green+"\tRedownloaded", addr, "found ", len(abis), " Functions"+colors.Off)
		}
	}

	return nil
}
