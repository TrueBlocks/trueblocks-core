package monitorsPkg

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
	"fmt"
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/cmd/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

type MonitorsOptions struct {
	Addrs       []string
	Appearances bool
	Count       bool
	Clean       bool
	Delete      bool
	Undelete    bool
	Remove      bool
	FirstBlock  uint64
	LastBlock   uint64
	Globals     globals.GlobalOptionsType
}

func (opts *MonitorsOptions) TestLog() {
	logger.TestLog(len(opts.Addrs) > 0, "Addrs: ", opts.Addrs)
	logger.TestLog(opts.Appearances, "Appearances: ", opts.Appearances)
	logger.TestLog(opts.Count, "Count: ", opts.Count)
	logger.TestLog(opts.Clean, "Clean: ", opts.Clean)
	logger.TestLog(opts.Delete, "Delete: ", opts.Delete)
	logger.TestLog(opts.Undelete, "Undelete: ", opts.Undelete)
	logger.TestLog(opts.Remove, "Remove: ", opts.Remove)
	logger.TestLog(opts.FirstBlock != 0, "FirstBlock: ", opts.FirstBlock)
	logger.TestLog(opts.LastBlock != utils.NOPOS, "LastBlock: ", opts.LastBlock)
	opts.Globals.TestLog()
}

func (opts *MonitorsOptions) ToDashStr() string {
	options := ""
	if opts.Appearances {
		options += " --appearances"
	}
	if opts.Count {
		options += " --count"
	}
	if opts.Clean {
		options += " --clean"
	}
	if opts.FirstBlock > 0 {
		options += " --first_block " + fmt.Sprintf("%d", opts.FirstBlock)
	}
	if opts.LastBlock > 0 {
		options += " --last_block " + fmt.Sprintf("%d", opts.LastBlock)
	}
	if opts.Delete {
		options += " --delete"
	}
	if opts.Remove {
		options += " --remove"
	}
	if opts.Undelete {
		options += " --undelete"
	}
	options += " " + strings.Join(opts.Addrs, " ")
	return options
}

func FromRequest(w http.ResponseWriter, r *http.Request) *MonitorsOptions {
	opts := &MonitorsOptions{}
	for key, value := range r.URL.Query() {
		switch key {
		case "addrs":
			opts.Addrs = append(opts.Addrs, value...)
		case "appearances":
			opts.Appearances = true
		case "count":
			opts.Count = true
		case "clean":
			opts.Clean = true
		case "delete":
			opts.Delete = true
		case "undelete":
			opts.Undelete = true
		case "remove":
			opts.Remove = true
		case "first_block":
			opts.FirstBlock = globals.ToUint64(value[0])
		case "last_block":
			opts.LastBlock = globals.ToUint64(value[0])
		}
	}
	opts.Globals = *globals.FromRequest(w, r)

	return opts
}
