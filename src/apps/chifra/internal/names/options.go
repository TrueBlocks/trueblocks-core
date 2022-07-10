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
	// a space separated list of one or more search terms
	Terms []string `json:"terms,omitempty"`
	// expand search to include all fields (search name, address, and symbol otherwise)
	Expand bool `json:"expand,omitempty"`
	// do case-sensitive search
	MatchCase bool `json:"matchCase,omitempty"`
	// include all accounts in the search
	All bool `json:"all,omitempty"`
	// include your custom named accounts
	Custom bool `json:"custom,omitempty"`
	// include prefund accounts
	Prefund bool `json:"prefund,omitempty"`
	// include well know token and airdrop addresses in the search
	Named bool `json:"named,omitempty"`
	// display only addresses in the results (useful for scripting)
	Addr bool `json:"addr,omitempty"`
	// display collections data
	Collections bool `json:"collections,omitempty"`
	// export the list of tags and subtags only
	Tags bool `json:"tags,omitempty"`
	// for editCmd only, is the edited name a custom name or not
	ToCustom bool `json:"toCustom,omitempty"`
	// clean the data (addrs to lower case, sort by addr)
	Clean bool `json:"clean,omitempty"`
	// an address assumed to be a token, added automatically to names database if true
	Autoname string `json:"autoname,omitempty"`
	// create a new name record
	Create bool `json:"create,omitempty"`
	// edit an existing name
	Update bool `json:"update,omitempty"`
	// delete a name, but do not remove it
	Delete bool `json:"delete,omitempty"`
	// undelete a previously deleted name
	Undelete bool `json:"undelete,omitempty"`
	// remove a previously deleted name
	Remove bool `json:"remove,omitempty"`
	// the global options
	Globals globals.GlobalOptions `json:"globals,omitempty"`
	// an error flag if needed
	BadFlag error `json:"badFlag,omitempty"`
}

var namesCmdLineOptions NamesOptions

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
	logger.TestLog(opts.Collections, "Collections: ", opts.Collections)
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
	b, _ := json.MarshalIndent(opts, "", "\t")
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
	if opts.Addr {
		options += " --addr"
	}
	if opts.Collections {
		options += " --collections"
	}
	if opts.Tags {
		options += " --tags"
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
	options += fmt.Sprintf("%s", "") // silence go compiler for auto gen
	return options
}

// namesFinishParseApi finishes the parsing for server invocations. Returns a new NamesOptions.
func namesFinishParseApi(w http.ResponseWriter, r *http.Request) *NamesOptions {
	opts := &NamesOptions{}
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
		case "collections":
			opts.Collections = true
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
	return &namesCmdLineOptions
}
