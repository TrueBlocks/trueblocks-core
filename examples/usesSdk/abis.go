package main

import (
	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// DoAbis tests the abi sdk function
func DoAbis() {
	logger.Info("DoAbis")

	opts := sdk.AbisOptions{
		Addrs: []string{"uniswap.eth"},
	}

	if functions, _, err := opts.Abis(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveAndClean[types.Function]("usesSDK/abis.json", functions, &opts, func() error {
			_, _, err := opts.Abis()
			return err
		}); err != nil {
			logger.Error(err)
		}
	}
}
