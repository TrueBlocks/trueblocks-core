package main

import (
	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// DoState tests the state sdk function
func DoState() {
	logger.Info("DoState")

	opts := sdk.StateOptions{
		BlockIds: []string{"18000000"},
		Addrs:    []string{"unchainedindex.eth"},
		Globals: sdk.Globals{
			Verbose: true,
		},
	}
	opts.Caching(sdk.CacheOn)

	if state, _, err := opts.State(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveAndClean[types.State]("usesSDK/state.json", state, &opts, func() error {
			_, _, err := opts.State()
			return err
		}); err != nil {
			logger.Error(err)
		}
	}

	opts.Articulate = true
	opts.Call = "manifestHashMap(0x02f2b09b33fdbd406ead954a31f98bd29a2a3492,\"mainnet\")"
	if stateResult, _, err := opts.StateCall(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveAndClean[types.Result]("usesSDK/stateResult.json", stateResult, &opts, func() error {
			_, _, err := opts.StateCall()
			return err
		}); err != nil {
			logger.Error(err)
		}
	}
}
