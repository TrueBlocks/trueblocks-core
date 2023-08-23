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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
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
	Globals globals.GlobalOptions `json:"globals,omitempty"` // The global options
	Conn    *rpc.Connection       `json:"conn,omitempty"`    // The connection to the RPC server
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
	opts.Conn.TestLog(opts.getCaches())
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
		default:
			if !copy.Globals.Caps.HasKey(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "abis")
			}
		}
	}
	opts.Conn = opts.Globals.FinishParseApi(w, r, opts.getCaches())

	// EXISTING_CODE
	// EXISTING_CODE
	opts.Addrs, _ = opts.Conn.GetEnsAddresses(opts.Addrs)

	return opts
}

// abisFinishParse finishes the parsing for command line invocations. Returns a new AbisOptions.
func abisFinishParse(args []string) *AbisOptions {
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
	for _, arg := range args {
		if base.IsValidAddress(arg) {
			opts.Addrs = append(opts.Addrs, arg)
		}
	}
	// EXISTING_CODE
	opts.Addrs, _ = opts.Conn.GetEnsAddresses(opts.Addrs)
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

func (opts *AbisOptions) getCaches() (m map[string]bool) {
	// EXISTING_CODE
	// EXISTING_CODE
	return
}

// EXISTING_CODE
// EXISTING_CODE
