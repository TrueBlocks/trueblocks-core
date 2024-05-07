package main

import (
	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// DoList tests the list sdk function
func DoList() {
	opts := sdk.ListOptions{
		Addrs: []string{testAddrs[0]},
	}
	ShowHeader("DoList", &opts)

	if appearances, _, err := opts.List(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Appearance]("sdkFuzzer/list.json", appearances); err != nil {
			logger.Error(err)
		}
	}

	if appearancesCount, _, err := opts.ListCount(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.AppearanceCount]("sdkFuzzer/listCount.json", appearancesCount); err != nil {
			logger.Error(err)
		}
	}

	if bounds, _, err := opts.ListBounds(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Bounds]("sdkFuzzer/listBounds.json", bounds); err != nil {
			logger.Error(err)
		}
	}
}
