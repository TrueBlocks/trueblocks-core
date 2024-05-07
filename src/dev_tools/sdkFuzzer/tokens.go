package main

import (
	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// DoTokens tests the tokens sdk function
func DoTokens() {
	opts := sdk.TokensOptions{
		Addrs: []string{"ens.eth", "trueblocks.eth"},
	}
	ShowHeader("DoTokens", &opts)

	if tokens, _, err := opts.Tokens(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveAndClean[types.Token]("sdkFuzzer/tokens.json", tokens, &opts, func() error {
			_, _, err := opts.Tokens()
			return err
		}); err != nil {
			logger.Error(err)
		}
	}
}
