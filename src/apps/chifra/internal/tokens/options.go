package tokens

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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/cmd/root"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

type TokensOptionsType struct {
	Addrs2  []string
	Blocks  []string
	Parts   []string
	ByAcct  bool
	NoZero  bool
	Globals root.RootOptionsType
}

var Options TokensOptionsType

func (opts *TokensOptionsType) TestLog() {
	logger.TestLog(len(opts.Addrs2) > 0, "Addrs2: ", opts.Addrs2)
	logger.TestLog(len(opts.Blocks) > 0, "Blocks: ", opts.Blocks)
	logger.TestLog(len(opts.Parts) > 0, "Parts: ", opts.Parts)
	logger.TestLog(opts.ByAcct, "ByAcct: ", opts.ByAcct)
	logger.TestLog(opts.NoZero, "NoZero: ", opts.NoZero)
	opts.Globals.TestLog()
}

func FromRequest(r *http.Request) *TokensOptionsType {
	opts := &TokensOptionsType{}
	for key, value := range r.URL.Query() {
		switch key {
		case "addrs2":
			opts.Addrs2 = append(opts.Addrs2, value...)
		case "blocks":
			opts.Blocks = append(opts.Blocks, value...)
		case "parts":
			opts.Parts = append(opts.Parts, value...)
		case "byacct":
			opts.ByAcct = true
		case "nozero":
			opts.NoZero = true
		}
	}
	opts.Globals = *root.FromRequest(r)

	return opts
}
