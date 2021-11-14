package statusPkg

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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

type StatusOptions struct {
	Modes     []string
	Details   bool
	Types     []string
	Depth     uint64
	Report    bool
	Terse     bool
	Migrate   []string
	GetConfig bool
	SetConfig bool
	TestStart uint64
	TestEnd   uint64
	Globals   globals.GlobalOptionsType
}

func (opts *StatusOptions) TestLog() {
	logger.TestLog(len(opts.Modes) > 0, "Modes: ", opts.Modes)
	logger.TestLog(opts.Details, "Details: ", opts.Details)
	logger.TestLog(len(opts.Types) > 0, "Types: ", opts.Types)
	logger.TestLog(opts.Depth != utils.NOPOS, "Depth: ", opts.Depth)
	logger.TestLog(opts.Terse, "Terse: ", opts.Terse)
	logger.TestLog(len(opts.Migrate) > 0, "Migrate: ", opts.Migrate)
	logger.TestLog(opts.GetConfig, "GetConfig: ", opts.GetConfig)
	logger.TestLog(opts.SetConfig, "SetConfig: ", opts.SetConfig)
	logger.TestLog(opts.TestStart != 0, "TestStart: ", opts.TestStart)
	logger.TestLog(opts.TestEnd != utils.NOPOS, "TestEnd: ", opts.TestEnd)
	opts.Globals.TestLog()
}

func FromRequest(w http.ResponseWriter, r *http.Request) *StatusOptions {
	opts := &StatusOptions{}
	for key, value := range r.URL.Query() {
		switch key {
		case "modes":
			opts.Modes = append(opts.Modes, value...)
		case "details":
			opts.Details = true
		case "types":
			opts.Types = append(opts.Types, value...)
		case "depth":
			opts.Depth = globals.ToUint64(value[0])
		case "report":
			opts.Report = true
		case "terse":
			opts.Terse = true
		case "migrate":
			opts.Migrate = append(opts.Migrate, value...)
		case "get_config":
			opts.GetConfig = true
		case "set_config":
			opts.SetConfig = true
		case "test_start":
			opts.TestStart = globals.ToUint64(value[0])
		case "test_end":
			opts.TestEnd = globals.ToUint64(value[0])
		}
	}
	opts.Globals = *globals.FromRequest(w, r)

	return opts
}
