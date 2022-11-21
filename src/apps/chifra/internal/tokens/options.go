// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package tokensPkg

import (
	"encoding/json"
	"fmt"
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient/ens"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

// TokensOptions provides all command options for the chifra tokens command.
type TokensOptions struct {
	Addrs    []string                 `json:"addrs,omitempty"`    // Two or more addresses (0x...), the first is an ERC20 token, balances for the rest are reported
	Blocks   []string                 `json:"blocks,omitempty"`   // An optional list of one or more blocks at which to report balances, defaults to 'latest'
	BlockIds []identifiers.Identifier `json:"blockIds,omitempty"` // Block identifiers
	Parts    []string                 `json:"parts,omitempty"`    // Which parts of the token information to retrieve
	ByAcct   bool                     `json:"byAcct,omitempty"`   // Consider each address an ERC20 token except the last, whose balance is reported for each token
	NoZero   bool                     `json:"noZero,omitempty"`   // Suppress the display of zero balance accounts
	Globals  globals.GlobalOptions    `json:"globals,omitempty"`  // The global options
	BadFlag  error                    `json:"badFlag,omitempty"`  // An error flag if needed
}

var defaultTokensOptions = TokensOptions{}

// testLog is used only during testing to export the options for this test case.
func (opts *TokensOptions) testLog() {
	logger.TestLog(len(opts.Addrs) > 0, "Addrs: ", opts.Addrs)
	logger.TestLog(len(opts.Blocks) > 0, "Blocks: ", opts.Blocks)
	logger.TestLog(len(opts.Parts) > 0, "Parts: ", opts.Parts)
	logger.TestLog(opts.ByAcct, "ByAcct: ", opts.ByAcct)
	logger.TestLog(opts.NoZero, "NoZero: ", opts.NoZero)
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *TokensOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "  ")
	return string(b)
}

// getEnvStr allows for custom environment strings when calling to the system (helps debugging).
func (opts *TokensOptions) getEnvStr() []string {
	envStr := []string{}
	// EXISTING_CODE
	// EXISTING_CODE
	return envStr
}

// toCmdLine converts the option to a command line for calling out to the system.
func (opts *TokensOptions) toCmdLine() string {
	options := ""
	for _, part := range opts.Parts {
		options += " --parts " + part
	}
	if opts.ByAcct {
		options += " --by_acct"
	}
	if opts.NoZero {
		options += " --no_zero"
	}
	options += " " + strings.Join(opts.Addrs, " ")
	options += " " + strings.Join(opts.Blocks, " ")
	// EXISTING_CODE
	// EXISTING_CODE
	options += fmt.Sprintf("%s", "") // silence compiler warning for auto gen
	return options
}

// tokensFinishParseApi finishes the parsing for server invocations. Returns a new TokensOptions.
func tokensFinishParseApi(w http.ResponseWriter, r *http.Request) *TokensOptions {
	copy := defaultTokensOptions
	opts := &copy
	for key, value := range r.URL.Query() {
		switch key {
		case "addrs":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Addrs = append(opts.Addrs, s...)
			}
		case "blocks":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Blocks = append(opts.Blocks, s...)
			}
		case "parts":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Parts = append(opts.Parts, s...)
			}
		case "byAcct":
			opts.ByAcct = true
		case "noZero":
			opts.NoZero = true
		default:
			if !globals.IsGlobalOption(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "tokens")
				return opts
			}
		}
	}
	opts.Globals = *globals.GlobalsFinishParseApi(w, r)
	// EXISTING_CODE
	opts.Addrs = ens.ConvertEns(opts.Globals.Chain, opts.Addrs)
	// EXISTING_CODE

	return opts
}

// tokensFinishParse finishes the parsing for command line invocations. Returns a new TokensOptions.
func tokensFinishParse(args []string) *TokensOptions {
	opts := GetOptions()
	opts.Globals.FinishParse(args)
	defFmt := "txt"
	// EXISTING_CODE
	for _, arg := range args {
		if validate.IsValidAddress(arg) {
			opts.Addrs = append(opts.Addrs, arg)
		} else {
			opts.Blocks = append(opts.Blocks, arg)
		}
	}
	opts.Addrs = ens.ConvertEns(opts.Globals.Chain, opts.Addrs)
	// EXISTING_CODE
	if len(opts.Globals.Format) == 0 || opts.Globals.Format == "none" {
		opts.Globals.Format = defFmt
	}
	return opts
}

func GetOptions() *TokensOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &defaultTokensOptions
}

func ResetOptions() {
	// We want to keep writer between command file calls
	w := GetOptions().Globals.Writer
	defaultTokensOptions = TokensOptions{}
	globals.SetDefaults(&defaultTokensOptions.Globals)
	defaultTokensOptions.Globals.Writer = w
}
