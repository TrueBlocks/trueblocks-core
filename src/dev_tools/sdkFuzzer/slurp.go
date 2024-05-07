package main

import (
	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// DoSlurp tests the slurp sdk function
func DoSlurp() {
	opts := sdk.SlurpOptions{
		Addrs:   []string{testAddrs[0]},
		PerPage: 10,
	}
	ShowHeader("DoSlurp", &opts)

	if slurp, _, err := opts.Slurp(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveAndClean[types.Slurp]("sdkFuzzer/slurp.json", slurp, &opts, func() error {
			_, _, err := opts.Slurp()
			return err
		}); err != nil {
			logger.Error(err)
		}
	}

	if appearances, _, err := opts.SlurpAppearances(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveAndClean[types.Appearance]("sdkFuzzer/slurpAppearances.json", appearances, &opts, func() error {
			_, _, err := opts.SlurpAppearances()
			return err
		}); err != nil {
			logger.Error(err)
		}
	}

	if counts, _, err := opts.SlurpCount(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveAndClean[types.SlurpCount]("sdkFuzzer/slurpCount.json", counts, &opts, func() error {
			_, _, err := opts.SlurpCount()
			return err
		}); err != nil {
			logger.Error(err)
		}
	}

}
