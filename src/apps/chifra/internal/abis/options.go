// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package abisPkg

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

// AbisOptions provides all command options for the chifra abis command.
type AbisOptions struct {
	Addrs   []string              `json:"addrs,omitempty"`   // A list of one or more smart contracts whose ABIs to display
	Known   bool                  `json:"known,omitempty"`   // Load common 'known' ABIs from cache
	Find    []string              `json:"find,omitempty"`    // Search for function or event declarations given a four- or 32-byte code(s)
	Hint    []string              `json:"hint,omitempty"`    // For the --find option only, provide hints to speed up the search
	Encode  string                `json:"encode,omitempty"`  // Generate the 32-byte encoding for a given cannonical function or event signature
	Clean   bool                  `json:"clean,omitempty"`   // Remove an abi file for an address or all zero-length files if no address is given
	Sol     bool                  `json:"sol,omitempty"`     // Please use the `solc --abi` tool instead
	Globals globals.GlobalOptions `json:"globals,omitempty"` // The global options
	Conn    *rpcClient.Options    `json:"conn,omitempty"`    // The connection to the RPC server
	BadFlag error                 `json:"badFlag,omitempty"` // An error flag if needed
	// EXISTING_CODE
	// EXISTING_CODE
}

var defaultAbisOptions = AbisOptions{}

// testLog is used only during testing to export the options for this test case.
func (opts *AbisOptions) testLog() {
	logger.TestLog(len(opts.Addrs) > 0, "Addrs: ", opts.Addrs)
	logger.TestLog(opts.Known, "Known: ", opts.Known)
	logger.TestLog(len(opts.Find) > 0, "Find: ", opts.Find)
	logger.TestLog(len(opts.Hint) > 0, "Hint: ", opts.Hint)
	logger.TestLog(len(opts.Encode) > 0, "Encode: ", opts.Encode)
	logger.TestLog(opts.Clean, "Clean: ", opts.Clean)
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *AbisOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "  ")
	return string(b)
}

// abisFinishParseApi finishes the parsing for server invocations. Returns a new AbisOptions.
func abisFinishParseApi(w http.ResponseWriter, r *http.Request) *AbisOptions {
	copy := defaultAbisOptions
	opts := &copy
	for key, value := range r.URL.Query() {
		switch key {
		case "addrs":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Addrs = append(opts.Addrs, s...)
			}
		case "known":
			opts.Known = true
		case "find":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Find = append(opts.Find, s...)
			}
		case "hint":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Hint = append(opts.Hint, s...)
			}
		case "encode":
			opts.Encode = value[0]
		case "clean":
			opts.Clean = true
		case "sol":
			opts.Sol = true
		default:
			if !copy.Globals.Caps.HasKey(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "abis")
				return opts
			}
		}
	}
	opts.Globals = *globals.GlobalsFinishParseApi(w, r)
	chain := opts.Globals.Chain
	caches := []string{}
	opts.Conn = rpcClient.NewConnection(chain, caches)

	// EXISTING_CODE
	opts.Addrs, _ = opts.Conn.GetAddressesFromEns(chain, opts.Addrs)
	// EXISTING_CODE

	return opts
}

// abisFinishParse finishes the parsing for command line invocations. Returns a new AbisOptions.
func abisFinishParse(args []string) *AbisOptions {
	opts := GetOptions()
	opts.Globals.FinishParse(args)
	defFmt := "txt"
	chain := opts.Globals.Chain
	caches := []string{}
	opts.Conn = rpcClient.NewConnection(chain, caches)

	// EXISTING_CODE
	if opts.Globals.IsApiMode() {
		defFmt = "json"
	}
	opts.Addrs, _ = opts.Conn.GetAddressesFromEns(chain, args)
	// EXISTING_CODE
	if len(opts.Globals.Format) == 0 || opts.Globals.Format == "none" {
		opts.Globals.Format = defFmt
	}

	return opts
}

func GetOptions() *AbisOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &defaultAbisOptions
}

func ResetOptions() {
	// We want to keep writer between command file calls
	w := GetOptions().Globals.Writer
	defaultAbisOptions = AbisOptions{}
	globals.SetDefaults(&defaultAbisOptions.Globals)
	defaultAbisOptions.Globals.Writer = w
	capabilities := caps.Default // Additional global caps for chifra abis
	// EXISTING_CODE
	// EXISTING_CODE
	defaultAbisOptions.Globals.Caps = capabilities
}

// EXISTING_CODE
// EXISTING_CODE
