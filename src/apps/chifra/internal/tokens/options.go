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
 * The file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

import (
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/cmd/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

type TokensOptions struct {
	Addrs2  []string
	Blocks  []string
	Parts   []string
	ByAcct  bool
	NoZero  bool
	Globals globals.GlobalOptionsType
}

func (opts *TokensOptions) TestLog() {
	logger.TestLog(len(opts.Addrs2) > 0, "Addrs2: ", opts.Addrs2)
	logger.TestLog(len(opts.Blocks) > 0, "Blocks: ", opts.Blocks)
	logger.TestLog(len(opts.Parts) > 0, "Parts: ", opts.Parts)
	logger.TestLog(opts.ByAcct, "ByAcct: ", opts.ByAcct)
	logger.TestLog(opts.NoZero, "NoZero: ", opts.NoZero)
	opts.Globals.TestLog()
}

func (opts *TokensOptions) ToDashStr() string {
	options := ""
	for _, t := range opts.Parts {
		options += " --parts " + t
	}
	if opts.ByAcct {
		options += " --by_acct"
	}
	if opts.NoZero {
		options += " --no_zero"
	}
	options += " " + strings.Join(opts.Addrs2, " ")
	return options
}

func FromRequest(w http.ResponseWriter, r *http.Request) *TokensOptions {
	opts := &TokensOptions{}
	for key, value := range r.URL.Query() {
		switch key {
		case "addrs2":
			opts.Addrs2 = append(opts.Addrs2, value...)
		case "blocks":
			opts.Blocks = append(opts.Blocks, value...)
		case "parts":
			opts.Parts = append(opts.Parts, value...)
		case "by_acct":
			opts.ByAcct = true
		case "no_zero":
			opts.NoZero = true
		}
	}
	opts.Globals = *globals.FromRequest(w, r)

	return opts
}
