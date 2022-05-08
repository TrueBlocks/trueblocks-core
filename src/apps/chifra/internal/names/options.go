// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package namesPkg

import (
	"fmt"
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient/ens"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

type NamesOptions struct {
	Terms       []string
	Expand      bool
	MatchCase   bool
	All         bool
	Custom      bool
	Prefund     bool
	Named       bool
	Addr        bool
	Collections bool
	Tags        bool
	ToCustom    bool
	Clean       bool
	Autoname    string
	Create      bool
	Update      bool
	Delete      bool
	Undelete    bool
	Remove      bool
	Globals     globals.GlobalOptions
	BadFlag     error
}

var namesCmdLineOptions NamesOptions

func (opts *NamesOptions) TestLog() {
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

func (opts *NamesOptions) ToCmdLine() string {
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
	options += fmt.Sprintf("%s", "") // silence go compiler for auto gen
	return options
}

func FromRequest(w http.ResponseWriter, r *http.Request) *NamesOptions {
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
	opts.Globals = *globals.FromRequest(w, r)
	// EXISTING_CODE
	opts.Terms = ens.ConvertEns(opts.Globals.Chain, opts.Terms)
	// EXISTING_CODE

	return opts
}

func NamesFinishParse(args []string) *NamesOptions {
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
