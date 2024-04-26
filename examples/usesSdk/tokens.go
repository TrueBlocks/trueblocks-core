package main

import (
	"bytes"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

// DoTokens tests the When sdk function
func DoTokens() {
	logger.Info("DoTokens")

	opts := sdk.TokensOptions{
		Addrs: []string{"ens.eth", "trueblocks.eth"},
	}

	buf := bytes.Buffer{}
	if err := opts.TokensBytes(&buf); err != nil {
		logger.Fatal(err)
	}

	file.StringToAsciiFile("usesSDK/tokens.json", buf.String())
	fmt.Println(buf.String())
}
// func (opts *TokensOptions) Tokens() ([]types.Token, *types.MetaData, error) {
