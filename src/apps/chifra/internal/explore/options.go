// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package explorePkg

import (
	"encoding/json"
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

// ExploreOptions provides all command options for the chifra explore command.
type ExploreOptions struct {
	Terms   []string              `json:"terms,omitempty"`   // One or more address, name, block, or transaction identifier
	Local   bool                  `json:"local,omitempty"`   // Open the local TrueBlocks explorer
	Google  bool                  `json:"google,omitempty"`  // Search google excluding popular blockchain explorers
	Globals globals.GlobalOptions `json:"globals,omitempty"` // The global options
	Conn    *rpcClient.Options    `json:"conn,omitempty"`    // The connection to the RPC server
	BadFlag error                 `json:"badFlag,omitempty"` // An error flag if needed
	// EXISTING_CODE
	// EXISTING_CODE
}

var defaultExploreOptions = ExploreOptions{}

// testLog is used only during testing to export the options for this test case.
func (opts *ExploreOptions) testLog() {
	logger.TestLog(len(opts.Terms) > 0, "Terms: ", opts.Terms)
	logger.TestLog(opts.Local, "Local: ", opts.Local)
	logger.TestLog(opts.Google, "Google: ", opts.Google)
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *ExploreOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "  ")
	return string(b)
}

// exploreFinishParseApi finishes the parsing for server invocations. Returns a new ExploreOptions.
func exploreFinishParseApi(w http.ResponseWriter, r *http.Request) *ExploreOptions {
	copy := defaultExploreOptions
	opts := &copy
	for key, value := range r.URL.Query() {
		switch key {
		case "terms":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Terms = append(opts.Terms, s...)
			}
		case "local":
			opts.Local = true
		case "google":
			opts.Google = true
		default:
			if !copy.Globals.Caps.HasKey(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "explore")
				return opts
			}
		}
	}
	opts.Globals = *globals.GlobalsFinishParseApi(w, r)
	chain := opts.Globals.Chain
	caches := []string{}
	opts.Conn = rpcClient.NewConnection(chain, caches)

	// EXISTING_CODE
	opts.Terms, _ = opts.Conn.GetAddressesFromEns(chain, opts.Terms)
	// EXISTING_CODE

	return opts
}

// exploreFinishParse finishes the parsing for command line invocations. Returns a new ExploreOptions.
func exploreFinishParse(args []string) *ExploreOptions {
	opts := GetOptions()
	opts.Globals.FinishParse(args)
	defFmt := "txt"
	chain := opts.Globals.Chain
	caches := []string{}
	opts.Conn = rpcClient.NewConnection(chain, caches)

	// EXISTING_CODE
	opts.Terms, _ = opts.Conn.GetAddressesFromEns(chain, args)
	// EXISTING_CODE
	if len(opts.Globals.Format) == 0 || opts.Globals.Format == "none" {
		opts.Globals.Format = defFmt
	}

	return opts
}

func GetOptions() *ExploreOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &defaultExploreOptions
}

func ResetOptions() {
	// We want to keep writer between command file calls
	w := GetOptions().Globals.Writer
	defaultExploreOptions = ExploreOptions{}
	globals.SetDefaults(&defaultExploreOptions.Globals)
	defaultExploreOptions.Globals.Writer = w
	capabilities := caps.Default // Additional global caps for chifra explore
	// EXISTING_CODE
	capabilities = capabilities.Remove(caps.Fmt)
	capabilities = capabilities.Remove(caps.NoHeader)
	capabilities = capabilities.Remove(caps.Output)
	capabilities = capabilities.Remove(caps.Append)
	// EXISTING_CODE
	defaultExploreOptions.Globals.Caps = capabilities
}

// EXISTING_CODE
// EXISTING_CODE
