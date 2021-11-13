package when

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

type WhenOptionsType struct {
	Blocks     []string
	List       bool
	Timestamps bool
	Check      bool
	Fix        bool
	Count      bool
	Globals    root.RootOptionsType
}

var Options WhenOptionsType

func (opts *WhenOptionsType) TestLog() {
	logger.TestLog(len(opts.Blocks) > 0, "Blocks: ", opts.Blocks)
	logger.TestLog(opts.List, "List: ", opts.List)
	logger.TestLog(opts.Timestamps, "Timestamps: ", opts.Timestamps)
	logger.TestLog(opts.Check, "Check: ", opts.Check)
	logger.TestLog(opts.Fix, "Fix: ", opts.Fix)
	logger.TestLog(opts.Count, "Count: ", opts.Count)
	opts.Globals.TestLog()
}

func FromRequest(r *http.Request) *WhenOptionsType {
	opts := &WhenOptionsType{}
	for key, value := range r.URL.Query() {
		switch key {
		case "blocks":
			opts.Blocks = append(opts.Blocks, value...)
		case "list":
			opts.List = true
		case "timestamps":
			opts.Timestamps = true
		case "check":
			opts.Check = true
		case "fix":
			opts.Fix = true
		case "count":
			opts.Count = true
		}
	}
	opts.Globals = *root.FromRequest(r)

	return opts
}
