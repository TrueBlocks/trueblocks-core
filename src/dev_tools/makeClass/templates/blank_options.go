package [{ROUTE}]Pkg

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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/cmd/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
[{IMPORTS}])

type [{PROPER}]OptionsType struct {
[{OPT_FIELDS}]
}

var Options [{PROPER}]OptionsType

func (opts *[{PROPER}]OptionsType) TestLog() {
[{TEST_LOGS}]	opts.Globals.TestLog()
}

func FromRequest(r *http.Request) *[{PROPER}]OptionsType {
	opts := &[{PROPER}]OptionsType{}
	for key, value := range r.URL.Query() {
		switch key {
[{REQUEST_OPTS}]		}
	}
	opts.Globals = *globals.FromRequest(r)

	return opts
}
