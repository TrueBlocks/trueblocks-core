package main

import (
	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// DoTokens tests the tokens sdk function
func DoTokens() {
	file.EstablishFolder("sdkFuzzer-output/tokens")
	opts := sdk.TokensOptions{
		Addrs:    []string{"ens.eth", "trueblocks.eth"},
		BlockIds: []string{"10092000"},
	}
	ShowHeader("DoTokens", &opts)

	globs := noCache(noRaw(noEther(globals)))
	parts := []sdk.TokensParts{
		sdk.NoTP,
		sdk.TPName,
		sdk.TPSymbol,
		sdk.TPDecimals,
		sdk.TPTotalSupply,
		sdk.TPVersion,
		sdk.TPSome,
		sdk.TPAll,
	}
	changes := []bool{false} // , true}
	noZeros := []bool{false, true}

	for _, c := range changes {
		for _, z := range noZeros {
			for _, p := range parts {
				baseFn := "tokens/tokens"
				if c {
					baseFn += "-changes"
				}
				if z {
					baseFn += "-noZero"
				}
				if p != sdk.NoTP {
					baseFn += "-" + p.String()
				}
				opts.Changes = c
				opts.NoZero = z
				opts.Parts = p
				for _, g := range globs {
					opts.Globals = g
					opts.ByAcct = false
					opts.Addrs = []string{"ens.eth", "trueblocks.eth"}
					fn := getFilename(baseFn, &opts.Globals)
					TestTokens("tokens", fn, &opts)
					// TODO: I don't think this actually works
					opts.ByAcct = true
					// opts.Addrs = []string{"trueblocks.eth", "ens.eth"}
					fn = getFilename(baseFn+"-byacct", &opts.Globals)
					TestTokens("tokens", fn, &opts)
				}
			}
		}
	}
	// ByAcct   bool        `json:"byAcct,omitempty"`
}

func TestTokens(which, fn string, opts *sdk.TokensOptions) {
	switch which {
	case "tokens":
		if tokens, _, err := opts.Tokens(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Token](fn, tokens); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	}
}
