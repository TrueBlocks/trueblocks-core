// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package namesPkg

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

// NamesOptions provides all command options for the chifra names command.
type NamesOptions struct {
	Terms     []string              `json:"terms,omitempty"`     // A space separated list of one or more search terms
	Expand    bool                  `json:"expand,omitempty"`    // Expand search to include all fields (search name, address, and symbol otherwise)
	MatchCase bool                  `json:"matchCase,omitempty"` // Do case-sensitive search
	All       bool                  `json:"all,omitempty"`       // Include all (including custom) names in the search
	Custom    bool                  `json:"custom,omitempty"`    // Include only custom named account in the search
	Prefund   bool                  `json:"prefund,omitempty"`   // Include prefund accounts in the search
	Named     bool                  `json:"named,omitempty"`     // Include well know token and airdrop addresses in the search
	Addr      bool                  `json:"addr,omitempty"`      // Display only addresses in the results (useful for scripting, assumes --no_header)
	Tags      bool                  `json:"tags,omitempty"`      // Export the list of tags and subtags only
	ToCustom  bool                  `json:"toCustom,omitempty"`  // For editCmd only, is the edited name a custom name or not
	Clean     bool                  `json:"clean,omitempty"`     // Clean the data (addrs to lower case, sort by addr)
	Autoname  string                `json:"autoname,omitempty"`  // An address assumed to be a token, added automatically to names database if true
	Create    bool                  `json:"create,omitempty"`    // Create a new name record
	Update    bool                  `json:"update,omitempty"`    // Edit an existing name
	Delete    bool                  `json:"delete,omitempty"`    // Delete a name, but do not remove it
	Undelete  bool                  `json:"undelete,omitempty"`  // Undelete a previously deleted name
	Remove    bool                  `json:"remove,omitempty"`    // Remove a previously deleted name
	Globals   globals.GlobalOptions `json:"globals,omitempty"`   // The global options
	BadFlag   error                 `json:"badFlag,omitempty"`   // An error flag if needed
}

var defaultNamesOptions = NamesOptions{}

// testLog is used only during testing to export the options for this test case.
func (opts *NamesOptions) testLog() {
	logger.TestLog(len(opts.Terms) > 0, "Terms: ", opts.Terms)
	logger.TestLog(opts.Expand, "Expand: ", opts.Expand)
	logger.TestLog(opts.MatchCase, "MatchCase: ", opts.MatchCase)
	logger.TestLog(opts.All, "All: ", opts.All)
	logger.TestLog(opts.Custom, "Custom: ", opts.Custom)
	logger.TestLog(opts.Prefund, "Prefund: ", opts.Prefund)
	logger.TestLog(opts.Named, "Named: ", opts.Named)
	logger.TestLog(opts.Addr, "Addr: ", opts.Addr)
	logger.TestLog(opts.Tags, "Tags: ", opts.Tags)
	logger.TestLog(opts.ToCustom, "ToCustom: ", opts.ToCustom)
	logger.TestLog(opts.Clean, "Clean: ", opts.Clean)
	logger.TestLog(len(opts.Autoname) > 0, "Autoname: ", opts.Autoname)
	logger.TestLog(opts.Create, "Create: ", opts.Create)
	logger.TestLog(opts.Update, "Update: ", opts.Update)
	logger.TestLog(opts.Delete, "Delete: ", opts.Delete)
	logger.TestLog(opts.Undelete, "Undelete: ", opts.Undelete)
	logger.TestLog(opts.Remove, "Remove: ", opts.Remove)
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *NamesOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "  ")
	return string(b)
}

// getEnvStr allows for custom environment strings when calling to the system (helps debugging).
func (opts *NamesOptions) getEnvStr() []string {
	envStr := []string{}
	// EXISTING_CODE
	// EXISTING_CODE
	return envStr
}

// toCmdLine converts the option to a command line for calling out to the system.
func (opts *NamesOptions) toCmdLine() string {
	options := ""
	if opts.Expand {
		options += " --expand"
	}
	if opts.MatchCase {
		options += " --match_case"
	}
	if opts.All {
		options += " --all"
	}
	if opts.Custom {
		options += " --custom"
	}
	if opts.Prefund {
		options += " --prefund"
	}
	if opts.Named {
		options += " --named"
	}
	if opts.ToCustom {
		options += " --to_custom"
	}
	if opts.Clean {
		options += " --clean"
	}
	if len(opts.Autoname) > 0 {
		options += " --autoname " + opts.Autoname
	}
	if opts.Create {
		options += " --create"
	}
	if opts.Update {
		options += " --update"
	}
	if opts.Delete {
		options += " --delete"
	}
	if opts.Undelete {
		options += " --undelete"
	}
	if opts.Remove {
		options += " --remove"
	}
	options += " " + strings.Join(opts.Terms, " ")
	// EXISTING_CODE
	// EXISTING_CODE
	options += fmt.Sprintf("%s", "") // silence compiler warning for auto gen
	return options
}

// namesFinishParseApi finishes the parsing for server invocations. Returns a new NamesOptions.
func namesFinishParseApi(w http.ResponseWriter, r *http.Request) *NamesOptions {
	copy := defaultNamesOptions
	opts := &copy
	for key, value := range r.URL.Query() {
		switch key {
		case "terms":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Terms = append(opts.Terms, s...)
			}
		case "expand":
			opts.Expand = true
		case "matchCase":
			opts.MatchCase = true
		case "all":
			opts.All = true
		case "custom":
			opts.Custom = true
		case "prefund":
			opts.Prefund = true
		case "named":
			opts.Named = true
		case "addr":
			opts.Addr = true
		case "tags":
			opts.Tags = true
		case "toCustom":
			opts.ToCustom = true
		case "clean":
			opts.Clean = true
		case "autoname":
			opts.Autoname = value[0]
		case "create":
			opts.Create = true
		case "update":
			opts.Update = true
		case "delete":
			opts.Delete = true
		case "undelete":
			opts.Undelete = true
		case "remove":
			opts.Remove = true
		default:
			if !globals.IsGlobalOption(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "names")
				return opts
			}
		}
	}
	opts.Globals = *globals.GlobalsFinishParseApi(w, r)
	// EXISTING_CODE
	opts.Terms = ens.ConvertEns(opts.Globals.Chain, opts.Terms)
	// EXISTING_CODE

	return opts
}

// namesFinishParse finishes the parsing for command line invocations. Returns a new NamesOptions.
func namesFinishParse(args []string) *NamesOptions {
	opts := GetOptions()
	opts.Globals.FinishParse(args)
	defFmt := "txt"
	// EXISTING_CODE
	opts.Terms = ens.ConvertEns(opts.Globals.Chain, args)
	// EXISTING_CODE
	if len(opts.Globals.Format) == 0 || opts.Globals.Format == "none" {
		opts.Globals.Format = defFmt
	}
	return opts
}

func GetOptions() *NamesOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &defaultNamesOptions
}

func ResetOptions() {
	// We want to keep writer between command file calls
	w := GetOptions().Globals.Writer
	defaultNamesOptions = NamesOptions{}
	globals.SetDefaults(&defaultNamesOptions.Globals)
	defaultNamesOptions.Globals.Writer = w
}
