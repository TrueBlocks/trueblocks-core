package servePkg

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
)

type ServeOptionsType struct {
	Port    string
	Globals globals.GlobalOptionsType
}

var Options ServeOptionsType

func (opts *ServeOptionsType) TestLog() {
	logger.TestLog(len(opts.Port) > 0, "Port: ", opts.Port)
	opts.Globals.TestLog()
}

func FromRequest(w http.ResponseWriter, r *http.Request) *ServeOptionsType {
	opts := &ServeOptionsType{}
	for key, value := range r.URL.Query() {
		switch key {
		case "port":
			opts.Port = value[0]
		}
	}
	opts.Globals = *globals.FromRequest(w, r)

	return opts
}
