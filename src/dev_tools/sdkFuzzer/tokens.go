// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */
package main

// EXISTING_CODE
import (
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	sdk "github.com/TrueBlocks/trueblocks-sdk/v4"
)

// EXISTING_CODE

// DoTokens tests the Tokens sdk function
func DoTokens() {
	file.EstablishFolder("sdkFuzzer-output/tokens")
	opts := sdk.TokensOptions{}
	ShowHeader("DoTokens", opts)

	globs := noEther(globals)
	// Option 'parts.list<enum>' is an emum
	byAcct := []bool{false, true}
	changes := []bool{false, true}
	noZero := []bool{false, true}
	// blocks is not fuzzed
	// Fuzz Loop
	// EXISTING_CODE
	_ = byAcct
	_ = changes
	changes = []bool{false} // , true}
	_ = globs
	globs = noCache(globs)
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
	opts = sdk.TokensOptions{
		Addrs:    []string{"ens.eth", "trueblocks.eth"},
		BlockIds: []string{"10092000"},
	}
	// tokens,command,default|caching|
	for _, c := range changes {
		for _, z := range noZero {
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
					TestTokens("tokens", "", fn, &opts)
					// TODO: I don't think this actually works
					opts.ByAcct = true
					// opts.Addrs = []string{"trueblocks.eth", "ens.eth"}
					fn = getFilename(baseFn+"-byacct", &opts.Globals)
					TestTokens("tokens", "", fn, &opts)
				}
			}
		}
	}
	// ByAcct   bool        `json:"byAcct,omitempty"`
	// EXISTING_CODE
	Wait()
}

func TestTokens(which, value, fn string, opts *sdk.TokensOptions) {
	fn = strings.Replace(fn, ".json", "-"+which+".json", 1)
	// EXISTING_CODE
	// EXISTING_CODE

	switch which {
	case "tokens":
		if tokens, _, err := opts.Tokens(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile(fn, tokens); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	default:
		ReportError(fn, opts, fmt.Errorf("unknown which: %s", which))
		logger.Fatal("Quitting...")
		return
	}
}

// EXISTING_CODE
// EXISTING_CODE
