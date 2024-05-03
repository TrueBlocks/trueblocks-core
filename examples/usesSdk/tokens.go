package main

import (
	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// DoTokens tests the tokens sdk function
func DoTokens() {
	logger.Info("DoTokens")

	opts := sdk.TokensOptions{
		Addrs: []string{"ens.eth", "trueblocks.eth"},
	}

	if tokens, _, err := opts.Tokens(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveAndClean[types.Token]("usesSDK/tokens.json", tokens, &opts, func() error {
			_, _, err := opts.Tokens()
			return err
		}); err != nil {
			logger.Error(err)
		}
	}
}
