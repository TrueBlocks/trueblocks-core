// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package abisPkg

import (
	"encoding/json"
	"fmt"
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient/ens"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

// AbisOptions provides all command options for the chifra abis command.
type AbisOptions struct {
	Addrs   []string              `json:"addrs,omitempty"`   // A list of one or more smart contracts whose ABIs to display
	Known   bool                  `json:"known,omitempty"`   // Load common 'known' ABIs from cache
	Sol     bool                  `json:"sol,omitempty"`     // Extract the abi definition from the provided .sol file(s)
	Find    []string              `json:"find,omitempty"`    // Search for function or event declarations given a four- or 32-byte code(s)
	Globals globals.GlobalOptions `json:"globals,omitempty"` // The global options
	BadFlag error                 `json:"badFlag,omitempty"` // An error flag if needed
}

var abisCmdLineOptions AbisOptions

// testLog is used only during testing to export the options for this test case.
func (opts *AbisOptions) testLog() {
	logger.TestLog(len(opts.Addrs) > 0, "Addrs: ", opts.Addrs)
	logger.TestLog(opts.Known, "Known: ", opts.Known)
	logger.TestLog(opts.Sol, "Sol: ", opts.Sol)
	logger.TestLog(len(opts.Find) > 0, "Find: ", opts.Find)
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *AbisOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "\t")
	return string(b)
}

// getEnvStr allows for custom environment strings when calling to the system (helps debugging).
func (opts *AbisOptions) getEnvStr() []string {
	envStr := []string{}
	// EXISTING_CODE
	// EXISTING_CODE
	return envStr
}

// toCmdLine converts the option to a command line for calling out to the system.
func (opts *AbisOptions) toCmdLine() string {
	options := ""
	if opts.Known {
		options += " --known"
	}
	if opts.Sol {
		options += " --sol"
	}
	for _, find := range opts.Find {
		options += " --find " + find
	}
	options += " " + strings.Join(opts.Addrs, " ")
	// EXISTING_CODE
	// EXISTING_CODE
	options += fmt.Sprintf("%s", "") // silence go compiler for auto gen
	return options
}

// abisFinishParseApi finishes the parsing for server invocations. Returns a new AbisOptions.
func abisFinishParseApi(w http.ResponseWriter, r *http.Request) *AbisOptions {
	opts := &AbisOptions{}
	for key, value := range r.URL.Query() {
		switch key {
		case "addrs":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Addrs = append(opts.Addrs, s...)
			}
		case "known":
			opts.Known = true
		case "sol":
			opts.Sol = true
		case "find":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Find = append(opts.Find, s...)
			}
		default:
			if !globals.IsGlobalOption(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "abis")
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

// abisFinishParse finishes the parsing for command line invocations. Returns a new AbisOptions.
func abisFinishParse(args []string) *AbisOptions {
	opts := GetOptions()
	opts.Globals.FinishParse(args)
	defFmt := "txt"
	// EXISTING_CODE
	if opts.Globals.ApiMode {
		defFmt = "api"
	}
	opts.Addrs = ens.ConvertEns(opts.Globals.Chain, args)
	// EXISTING_CODE
	if len(opts.Globals.Format) == 0 || opts.Globals.Format == "none" {
		opts.Globals.Format = defFmt
	}
	return opts
}

func GetOptions() *AbisOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &abisCmdLineOptions
}
