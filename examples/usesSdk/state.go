package main

import (
	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// DoState tests the state sdk function
func DoState() {
	opts := sdk.StateOptions{
		BlockIds: testBlocks,
		Addrs:    []string{"unchainedindex.eth"},
	}
	opts.Caching(sdk.CacheOn)

	if state, _, err := opts.State(); err != nil {
		logger.Fatal(err)
	} else {
		SaveAndClean[types.State]("usesSDK/state.json", state, &opts, func() error {
			_, _, err := opts.State()
			return err
		})
	}

	opts.Call = "manifestHashMap(trueblocks.eth,\"mainnet\")"
	if stateResult, _, err := opts.State(); err != nil {
		logger.Fatal(err)
	} else {
		SaveAndClean[types.State]("usesSDK/stateResult.json", stateResult, &opts, func() error {
			_, _, err := opts.State()
			return err
		})
	}
}
