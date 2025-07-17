package main

import (
	"fmt"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	sdk "github.com/TrueBlocks/trueblocks-sdk/v5"
)

func downloadAbis() error {
	logger.SetTestMode(false)
	defer logger.SetTestMode(true)

	logger.Info(colors.Yellow + "Downloading Abis..." + colors.Off)

	addrs := []string{
		"0x98ee18d7a1f7510b78b36f5a16471c7cd0c1c531",
		"0xd9db270c1b5e3bd161e8c8503c55ceabee709552",
		"0x7d655c57f71464b6f83811c55d84009cd9f5221c",
		"0x45f783cce6b7ff23b2ab2d70e416cdb7d6055f51",
		"0x9ba00d6856a4edf4665bca2c2309936572473b7e",
		"0xdbd27635a534a3d3169ef0498beb56fb9c937489",
		"0xbb9bc244d798123fde783fcc1c72d3bb8c189413",
		"0x8d12a197cb00d4747a1fe03395095ce2a5cc6819",
		"0xbb2b8038a1640196fbe3e38816f3e67cba72d940",
		"0x5c69bee701ef814a2b6a3edd4b1652cb9cc5aa6f",
		"0x2260fac5e5542a773aa44fbcfedf7c193bc2c599",
		"0xffa93aacf49297d51e211817452839052fdfb961",
		"0xef638b4305b8a1620f4e0e562e127f1181ae16d2",
	}

	for _, addr := range addrs {
		opts := sdk.AbisOptions{
			Addrs: []string{addr},
		}
		if abis, _, err := opts.Abis(); err != nil {
			logger.Fatal(err)
		} else if len(abis) > 1 {
			logger.InfoG(fmt.Sprintf("ABI for %s already valid with %d functions.", addr, len(abis)))
			continue
		} else {
			logger.InfoBY(fmt.Sprintf("Decaching invalid ABI %s.", addr))
			opts.Globals.Decache = true
			if _, _, err := opts.Abis(); err != nil {
				logger.Fatal(err)
			}
			logger.InfoBY(fmt.Sprintf("Redownloading %s...", addr))
			opts.Globals.Decache = false
			if abis, _, err = opts.Abis(); err != nil {
				logger.Fatal(err)
			} else {
				if len(abis) < 2 {
					logger.InfoY("Possibly rate limited. Sleeping...")
					time.Sleep(3 * time.Second)
					logger.InfoY("Trying again...")
					if abis, _, err = opts.Abis(); err != nil {
						logger.Fatal(err)
					}
				}
				logger.InfoG(fmt.Sprintf("Redownloaded %s found %d functions.", addr, len(abis)))
				time.Sleep(500 * time.Millisecond)
			}
		}
	}

	return nil
}
