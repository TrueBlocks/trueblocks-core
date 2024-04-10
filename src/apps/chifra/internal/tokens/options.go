// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package tokensPkg

import (
	// EXISTING_CODE
	"encoding/json"
	"io"
	"net/http"
	"net/url"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	// EXISTING_CODE
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
	Conn     *rpc.Connection          `json:"conn,omitempty"`     // The connection to the RPC server
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
	opts.Conn.TestLog(opts.getCaches())
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *TokensOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "  ")
	return string(b)
}

// tokensFinishParseApi finishes the parsing for server invocations. Returns a new TokensOptions.
func tokensFinishParseApi(w http.ResponseWriter, r *http.Request) *TokensOptions {
	values := r.URL.Query()
	if r.Header.Get("User-Agent") == "testRunner" {
		values.Set("testRunner", "true")
	}
	return TokensFinishParseInternal(w, values)
}

func TokensFinishParseInternal(w io.Writer, values url.Values) *TokensOptions {
	copy := defaultTokensOptions
	opts := &copy
	for key, value := range values {
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
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "tokens")
			}
		}
	}
	opts.Conn = opts.Globals.FinishParseApi(w, values, opts.getCaches())

	// EXISTING_CODE
	if len(opts.Addrs) == 1 && len(opts.Parts) == 0 {
		opts.Parts = append(opts.Parts, "all")
	}
	if len(opts.Blocks) == 0 {
		if opts.Globals.TestMode {
			opts.Blocks = []string{"17000000"}
		} else {
			opts.Blocks = []string{"latest"}
		}
	}
	// EXISTING_CODE
	opts.Addrs, _ = opts.Conn.GetEnsAddresses(opts.Addrs)

	return opts
}

// tokensFinishParse finishes the parsing for command line invocations. Returns a new TokensOptions.
func tokensFinishParse(args []string) *TokensOptions {
	// remove duplicates from args if any (not needed in api mode because the server does it).
	dedup := map[string]int{}
	if len(args) > 0 {
		tmp := []string{}
		for _, arg := range args {
			if value := dedup[arg]; value == 0 {
				tmp = append(tmp, arg)
			}
			dedup[arg]++
		}
		args = tmp
	}

	defFmt := "txt"
	opts := GetOptions()
	opts.Conn = opts.Globals.FinishParse(args, opts.getCaches())

	// EXISTING_CODE
	if len(args) > 0 {
		// The first argument in the list is the token, so not a dup...
		cnt := dedup[args[0]]
		if cnt > 1 {
			// The first token was present more than once, which is okay for
			// tokens who happen to own themselves, so add it back in.
			args = append(args, args[0])
		}

		// we need to separate blocks from addresses
		for _, arg := range args {
			if base.IsValidAddress(arg) {
				opts.Addrs = append(opts.Addrs, arg)
			} else {
				opts.Blocks = append(opts.Blocks, arg)
			}
		}
	}
	if len(opts.Addrs) == 1 && len(opts.Parts) == 0 {
		opts.Parts = append(opts.Parts, "all")
	}
	if len(opts.Blocks) == 0 {
		if opts.Globals.TestMode {
			opts.Blocks = []string{"17000000"}
		} else {
			opts.Blocks = []string{"latest"}
		}
	}
	// EXISTING_CODE
	opts.Addrs, _ = opts.Conn.GetEnsAddresses(opts.Addrs)
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

func ResetOptions(testMode bool) {
	// We want to keep writer between command file calls
	w := GetOptions().Globals.Writer
	defaultTokensOptions = TokensOptions{}
	globals.SetDefaults(&defaultTokensOptions.Globals)
	defaultTokensOptions.Globals.TestMode = testMode
	defaultTokensOptions.Globals.Writer = w
	var capabilities caps.Capability // capabilities for chifra tokens
	capabilities = capabilities.Add(caps.Default)
	capabilities = capabilities.Add(caps.Caching)
	// EXISTING_CODE
	// EXISTING_CODE
	defaultTokensOptions.Globals.Caps = capabilities
}

func (opts *TokensOptions) getCaches() (m map[string]bool) {
	// EXISTING_CODE
	m = map[string]bool{
		"tokens": true,
	}
	// EXISTING_CODE
	return
}

// EXISTING_CODE
// EXISTING_CODE
