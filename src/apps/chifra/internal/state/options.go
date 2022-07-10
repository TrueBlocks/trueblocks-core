// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package statePkg

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

// StateOptions provides all command options for the chifra state command.
type StateOptions struct {
	// one or more addresses (0x...) from which to retrieve balances
	Addrs []string `json:"addrs,omitempty"`
	// an optional list of one or more blocks at which to report balances, defaults to 'latest'
	Blocks []string `json:"blocks,omitempty"`
	// block identifiers
	BlockIds []identifiers.Identifier `json:"blockIds,omitempty"`
	// control which state to export
	Parts []string `json:"parts,omitempty"`
	// only report a balance when it changes from one block to the next
	Changes bool `json:"changes,omitempty"`
	// suppress the display of zero balance accounts
	NoZero bool `json:"noZero,omitempty"`
	// a bang-separated string consisting of address!4-byte!bytes
	Call string `json:"call,omitempty"`
	// for the --call option only, redirects calls to this implementation
	ProxyFor string `json:"proxyFor,omitempty"`
	// the global options
	Globals globals.GlobalOptions `json:"globals,omitempty"`
	// an error flag if needed
	BadFlag error `json:"badFlag,omitempty"`
}

var stateCmdLineOptions StateOptions

// testLog is used only during testing to export the options for this test case.
func (opts *StateOptions) testLog() {
	logger.TestLog(len(opts.Addrs) > 0, "Addrs: ", opts.Addrs)
	logger.TestLog(len(opts.Blocks) > 0, "Blocks: ", opts.Blocks)
	logger.TestLog(len(opts.Parts) > 0, "Parts: ", opts.Parts)
	logger.TestLog(opts.Changes, "Changes: ", opts.Changes)
	logger.TestLog(opts.NoZero, "NoZero: ", opts.NoZero)
	logger.TestLog(len(opts.Call) > 0, "Call: ", opts.Call)
	logger.TestLog(len(opts.ProxyFor) > 0, "ProxyFor: ", opts.ProxyFor)
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *StateOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "\t")
	return string(b)
}

// getEnvStr allows for custom environment strings when calling to the system (helps debugging).
func (opts *StateOptions) getEnvStr() []string {
	envStr := []string{}
	// EXISTING_CODE
	// EXISTING_CODE
	return envStr
}

// toCmdLine converts the option to a command line for calling out to the system.
func (opts *StateOptions) toCmdLine() string {
	options := ""
	for _, part := range opts.Parts {
		options += " --parts " + part
	}
	if opts.Changes {
		options += " --changes"
	}
	if opts.NoZero {
		options += " --no_zero"
	}
	if len(opts.Call) > 0 {
		options += " --call " + opts.Call
	}
	if len(opts.ProxyFor) > 0 {
		options += " --proxy_for " + opts.ProxyFor
	}
	options += " " + strings.Join(opts.Addrs, " ")
	options += " " + strings.Join(opts.Blocks, " ")
	// EXISTING_CODE
	// EXISTING_CODE
	options += fmt.Sprintf("%s", "") // silence go compiler for auto gen
	return options
}

// stateFinishParseApi finishes the parsing for server invocations. Returns a new StateOptions.
func stateFinishParseApi(w http.ResponseWriter, r *http.Request) *StateOptions {
	opts := &StateOptions{}
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
		case "changes":
			opts.Changes = true
		case "noZero":
			opts.NoZero = true
		case "call":
			opts.Call = value[0]
		case "proxyFor":
			opts.ProxyFor = value[0]
		default:
			if !globals.IsGlobalOption(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "state")
				return opts
			}
		}
	}
	opts.Globals = *globals.GlobalsFinishParseApi(w, r)
	// EXISTING_CODE
	opts.Addrs = ens.ConvertEns(opts.Globals.Chain, opts.Addrs)
	opts.ProxyFor = ens.ConvertOneEns(opts.Globals.Chain, opts.ProxyFor)
	// EXISTING_CODE

	return opts
}

// stateFinishParse finishes the parsing for command line invocations. Returns a new StateOptions.
func stateFinishParse(args []string) *StateOptions {
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
	opts.ProxyFor = ens.ConvertOneEns(opts.Globals.Chain, opts.ProxyFor)
	opts.Call = strings.Replace(opts.Call, "|", "!", -1)
	opts.Call = strings.Replace(opts.Call, " !", "!", -1)
	opts.Call = strings.Replace(opts.Call, "! ", "!", -1)
	parts := strings.Split(opts.Call, "!")
	if len(parts) > 0 {
		opts.Call = strings.Replace(opts.Call, parts[0], ens.ConvertOneEns(opts.Globals.Chain, parts[0]), -1)
	}
	// EXISTING_CODE
	if len(opts.Globals.Format) == 0 || opts.Globals.Format == "none" {
		opts.Globals.Format = defFmt
	}
	return opts
}

func GetOptions() *StateOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &stateCmdLineOptions
}
