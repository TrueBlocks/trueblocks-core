package tokensPkg

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

type TokensOptions struct {
	Addrs2  []string
	Blocks  []string
	Parts   []string
	ByAcct  bool
	NoZero  bool
	Globals globals.GlobalOptions
	BadFlag error
}

func (opts *TokensOptions) TestLog() {
	logger.TestLog(len(opts.Addrs2) > 0, "Addrs2: ", opts.Addrs2)
	logger.TestLog(len(opts.Blocks) > 0, "Blocks: ", opts.Blocks)
	logger.TestLog(len(opts.Parts) > 0, "Parts: ", opts.Parts)
	logger.TestLog(opts.ByAcct, "ByAcct: ", opts.ByAcct)
	logger.TestLog(opts.NoZero, "NoZero: ", opts.NoZero)
	opts.Globals.TestLog()
}

func (opts *TokensOptions) ToCmdLine() string {
	options := ""
	for _, part := range opts.Parts {
		options += " --parts " + part
	}
	if opts.ByAcct {
		options += " --by_acct"
	}
	if opts.NoZero {
		options += " --no_zero"
	}
	options += " " + strings.Join(opts.Addrs2, " ")
	options += fmt.Sprintf("%s", "") // silence go compiler for auto gen
	return options
}

func FromRequest(w http.ResponseWriter, r *http.Request) *TokensOptions {
	opts := &TokensOptions{}
	for key, value := range r.URL.Query() {
		switch key {
		case "addrs2":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Addrs2 = append(opts.Addrs2, s...)
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
		case "by_acct":
			opts.ByAcct = true
		case "no_zero":
			opts.NoZero = true
		default:
			if !globals.IsGlobalOption(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "tokens")
				return opts
			}
		}
	}
	opts.Globals = *globals.FromRequest(w, r)

	return opts
}

var Options TokensOptions

func TokensFinishParse(args []string) *TokensOptions {
	// EXISTING_CODE
	Options.Addrs2 = args
	// EXISTING_CODE
	return &Options
}
