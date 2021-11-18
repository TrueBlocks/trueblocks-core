package explorePkg

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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

type ExploreOptions struct {
	Terms   []string
	Local   bool
	Google  bool
	Globals globals.GlobalOptionsType
	BadFlag error
}

func (opts *ExploreOptions) TestLog() {
	logger.TestLog(len(opts.Terms) > 0, "Terms: ", opts.Terms)
	logger.TestLog(opts.Local, "Local: ", opts.Local)
	logger.TestLog(opts.Google, "Google: ", opts.Google)
	opts.Globals.TestLog()
}

func (opts *ExploreOptions) ToCmdLine() string {
	options := ""
	if opts.Local {
		options += " --local"
	}
	if opts.Google {
		options += " --google"
	}
	options += " " + strings.Join(opts.Terms, " ")
	options += fmt.Sprintf("%s", "") // silence go compiler for auto gen
	return options
}

func FromRequest(w http.ResponseWriter, r *http.Request) *ExploreOptions {
	opts := &ExploreOptions{}
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
			if !globals.IsGlobalOption(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "explore")
				return opts
			}
		}
	}
	opts.Globals = *globals.FromRequest(w, r)

	return opts
}

var Options ExploreOptions

func ExploreFinishParse(args []string) *ExploreOptions {
	Options.Terms = args
	// EXISTING_CODE
	// EXISTING_CODE
	return &Options
}