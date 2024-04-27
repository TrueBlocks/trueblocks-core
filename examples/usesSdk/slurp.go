package main

import (
	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// DoSlurp tests the When sdk function
func DoSlurp() {
	logger.Info("DoSlurp")

	opts := sdk.SlurpOptions{
		Addrs:   []string{testAddrs[0]},
		PerPage: 10,
	}

	if slurp, _, err := opts.Slurp(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveAndClean[types.Slurp]("usesSDK/slurp.json", slurp, &opts, func() error {
			_, _, err := opts.Slurp()
			return err
		}); err != nil {
			logger.Error(err)
		}
	}

	opts.Appearances = true
	if appearances, _, err := opts.SlurpAppearances(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveAndClean[types.Appearance]("usesSDK/slurpAppearances.json", appearances, &opts, func() error {
			_, _, err := opts.SlurpAppearances()
			return err
		}); err != nil {
			logger.Error(err)
		}
	}

	opts.Appearances = true
	opts.Count = true
	if counts, _, err := opts.SlurpCount(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveAndClean[types.SlurpCount]("usesSDK/slurpCount.json", counts, &opts, func() error {
			_, _, err := opts.SlurpCount()
			return err
		}); err != nil {
			logger.Error(err)
		}
	}

}
