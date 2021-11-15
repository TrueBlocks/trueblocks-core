package namesPkg

/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

import (
	"fmt"
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/cmd/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
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
	Delete      bool
	Update      bool
	Remove      bool
	Undelete    bool
	Globals     globals.GlobalOptionsType
}

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
	logger.TestLog(opts.Delete, "Delete: ", opts.Delete)
	logger.TestLog(opts.Update, "Update: ", opts.Update)
	logger.TestLog(opts.Remove, "Remove: ", opts.Remove)
	logger.TestLog(opts.Undelete, "Undelete: ", opts.Undelete)
	opts.Globals.TestLog()
}

func (opts *NamesOptions) ToDashStr() string {
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
	if opts.Delete {
		options += " --delete"
	}
	if opts.Update {
		options += " --update"
	}
	if opts.Remove {
		options += " --remove"
	}
	if opts.Undelete {
		options += " --undelete"
	}
	options += " " + strings.Join(opts.Terms, " ")
	options += fmt.Sprintf("%s", "") // auto gen only
	return options
}

func FromRequest(w http.ResponseWriter, r *http.Request) *NamesOptions {
	opts := &NamesOptions{}
	for key, value := range r.URL.Query() {
		switch key {
		case "terms":
			opts.Terms = append(opts.Terms, value...)
		case "expand":
			opts.Expand = true
		case "match_case":
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
		case "to_custom":
			opts.ToCustom = true
		case "clean":
			opts.Clean = true
		case "autoname":
			opts.Autoname = value[0]
		case "create":
			opts.Create = true
		case "delete":
			opts.Delete = true
		case "update":
			opts.Update = true
		case "remove":
			opts.Remove = true
		case "undelete":
			opts.Undelete = true
		}
	}
	opts.Globals = *globals.FromRequest(w, r)

	return opts
}
