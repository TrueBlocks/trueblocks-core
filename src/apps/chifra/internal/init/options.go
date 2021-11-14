package initPkg

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

type InitOptionsType struct {
	All     bool
	Globals root.GlobalOptionsType
}

var Options InitOptionsType

func (opts *InitOptionsType) TestLog() {
	logger.TestLog(opts.All, "All: ", opts.All)
	opts.Globals.TestLog()
}

func FromRequest(r *http.Request) *InitOptionsType {
	opts := &InitOptionsType{}
	for key, _ := range r.URL.Query() {
		switch key {
		case "all":
			opts.All = true
		}
	}
	opts.Globals = *root.FromRequest(r)

	return opts
}
