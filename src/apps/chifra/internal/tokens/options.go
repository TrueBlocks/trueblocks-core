// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package tokensPkg

import (
	"encoding/json"
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

// TokensOptions provides all command options for the chifra tokens command.
type TokensOptions struct {
	Addrs    []string                 `json:"addrs,omitempty"`    // Two or more addresses (0x...), the first is an ERC20 token, balances for the rest are reported
	Blocks   []string                 `json:"blocks,omitempty"`   // An optional list of one or more blocks at which to report balances, defaults to 'latest'
	BlockIds []identifiers.Identifier `json:"blockIds,omitempty"` // Block identifiers
	Parts    []string                 `json:"parts,omitempty"`    // Which parts of the token information to retrieve
	ByAcct   bool                     `json:"byAcct,omitempty"`   // Consider each address an ERC20 token except the last, whose balance is reported for each token
	Changes  bool                     `json:"changes,omitempty"`  // Only report a balance when it changes from one block to the next
	NoZero   bool                     `json:"noZero,omitempty"`   // Suppress the display of zero balance accounts
	Globals  globals.GlobalOptions    `json:"globals,omitempty"`  // The global options
	Conn     *rpcClient.Options       `json:"conn,omitempty"`     // The connection to the RPC server
	BadFlag  error                    `json:"badFlag,omitempty"`  // An error flag if needed
	// EXISTING_CODE
	// EXISTING_CODE
}

var defaultTokensOptions = TokensOptions{}

// testLog is used only during testing to export the options for this test case.
func (opts *TokensOptions) testLog() {
	logger.TestLog(len(opts.Addrs) > 0, "Addrs: ", opts.Addrs)
	logger.TestLog(len(opts.Blocks) > 0, "Blocks: ", opts.Blocks)
	logger.TestLog(len(opts.Parts) > 0, "Parts: ", opts.Parts)
	logger.TestLog(opts.ByAcct, "ByAcct: ", opts.ByAcct)
	logger.TestLog(opts.Changes, "Changes: ", opts.Changes)
	logger.TestLog(opts.NoZero, "NoZero: ", opts.NoZero)
	opts.Conn.TestLog()
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *TokensOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "  ")
	return string(b)
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
		case "changes":
			opts.Changes = true
		case "noZero":
			opts.NoZero = true
		default:
			if !copy.Globals.Caps.HasKey(key) {
				opts.Conn = &rpcClient.Options{}
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "tokens")
				return opts
			}
		}
	}
	opts.Globals = *globals.GlobalsFinishParseApi(w, r)
	chain := opts.Globals.Chain
	caches := []string{}
	opts.Conn = rpcClient.NewConnection(chain, caches)

	// EXISTING_CODE
	opts.Conn.EnableCaches(opts.Globals.Cache, false, false)
	opts.Addrs, _ = opts.Conn.GetAddressesFromEns(chain, opts.Addrs)
	if len(opts.Blocks) == 0 {
		if opts.Globals.TestMode {
			opts.Blocks = []string{"17000000"}
		} else {
			opts.Blocks = []string{"latest"}
		}
	}
	// EXISTING_CODE

	return opts
}

// tokensFinishParse finishes the parsing for command line invocations. Returns a new TokensOptions.
func tokensFinishParse(args []string) *TokensOptions {
	opts := GetOptions()
	opts.Globals.FinishParse(args)
	defFmt := "txt"
	chain := opts.Globals.Chain
	caches := []string{}
	opts.Conn = rpcClient.NewConnection(chain, caches)

	// EXISTING_CODE
	opts.Conn.EnableCaches(opts.Globals.Cache, false, false)
	if len(args) > 0 {
		dupMap := make(map[string]bool)
		for index, arg := range args {
			if !dupMap[arg] {
				if base.IsValidAddress(arg) {
					opts.Addrs = append(opts.Addrs, arg)
				} else {
					opts.Blocks = append(opts.Blocks, arg)
				}
			}
			if index > 0 {
				// the first item is the token, so never dupped
				dupMap[arg] = true
			}
		}
	}
	opts.Addrs, _ = opts.Conn.GetAddressesFromEns(chain, opts.Addrs)
	if len(opts.Blocks) == 0 {
		if opts.Globals.TestMode {
			opts.Blocks = []string{"17000000"}
		} else {
			opts.Blocks = []string{"latest"}
		}
	}
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
	capabilities := caps.Default // Additional global caps for chifra tokens
	// EXISTING_CODE
	capabilities = capabilities.Add(caps.Caching)
	// EXISTING_CODE
	defaultTokensOptions.Globals.Caps = capabilities
}

// EXISTING_CODE
// EXISTING_CODE
