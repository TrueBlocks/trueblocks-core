package main

import (
	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// DoList tests the When sdk function
func DoList() {
	logger.Info("DoList")

	opts := sdk.ListOptions{
		Addrs: []string{testAddrs[0]},
	}

	if appearances, _, err := opts.List(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveAndClean[types.Appearance]("usesSDK/list.json", appearances, &opts, func() error {
			_, _, err := opts.List()
			return err
		}); err != nil {
			logger.Error(err)
		}
	}

	if appearancesCount, _, err := opts.ListCount(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveAndClean[types.AppearanceCount]("usesSDK/listCount.json", appearancesCount, &opts, func() error {
			_, _, err := opts.ListCount()
			return err
		}); err != nil {
			logger.Error(err)
		}
	}

	if bounds, _, err := opts.ListBounds(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveAndClean[types.Bounds]("usesSDK/listBounds.json", bounds, &opts, func() error {
			_, _, err := opts.ListBounds()
			return err
		}); err != nil {
			logger.Error(err)
		}
	}
}
