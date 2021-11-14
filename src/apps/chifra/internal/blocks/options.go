package blocksPkg

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

type BlocksOptions struct {
	Blocks     []string
	Hashes     bool
	Uncles     bool
	Trace      bool
	Apps       bool
	Uniq       bool
	Logs       bool
	Emitter    []string
	Topic      []string
	Articulate bool
	BigRange   uint64
	Count      bool
	Cache      bool
	List       uint64
	ListCount  uint64
	Globals    globals.GlobalOptionsType
}

var Options BlocksOptions

func (opts *BlocksOptions) TestLog() {
	logger.TestLog(len(opts.Blocks) > 0, "Blocks: ", opts.Blocks)
	logger.TestLog(opts.Hashes, "Hashes: ", opts.Hashes)
	logger.TestLog(opts.Uncles, "Uncles: ", opts.Uncles)
	logger.TestLog(opts.Trace, "Trace: ", opts.Trace)
	logger.TestLog(opts.Apps, "Apps: ", opts.Apps)
	logger.TestLog(opts.Uniq, "Uniq: ", opts.Uniq)
	logger.TestLog(opts.Logs, "Logs: ", opts.Logs)
	logger.TestLog(len(opts.Emitter) > 0, "Emitter: ", opts.Emitter)
	logger.TestLog(len(opts.Topic) > 0, "Topic: ", opts.Topic)
	logger.TestLog(opts.Articulate, "Articulate: ", opts.Articulate)
	logger.TestLog(opts.BigRange != 500, "BigRange: ", opts.BigRange)
	logger.TestLog(opts.Count, "Count: ", opts.Count)
	logger.TestLog(opts.Cache, "Cache: ", opts.Cache)
	logger.TestLog(opts.List != 0, "List: ", opts.List)
	logger.TestLog(opts.ListCount != 20, "ListCount: ", opts.ListCount)
	opts.Globals.TestLog()
}

func FromRequest(w http.ResponseWriter, r *http.Request) *BlocksOptions {
	opts := &BlocksOptions{}
	for key, value := range r.URL.Query() {
		switch key {
		case "blocks":
			opts.Blocks = append(opts.Blocks, value...)
		case "hashes":
			opts.Hashes = true
		case "uncles":
			opts.Uncles = true
		case "trace":
			opts.Trace = true
		case "apps":
			opts.Apps = true
		case "uniq":
			opts.Uniq = true
		case "logs":
			opts.Logs = true
		case "emitter":
			opts.Emitter = append(opts.Emitter, value...)
		case "topic":
			opts.Topic = append(opts.Topic, value...)
		case "articulate":
			opts.Articulate = true
		case "bigrange":
			opts.BigRange = globals.ToUint64(value[0])
		case "count":
			opts.Count = true
		case "cache":
			opts.Cache = true
		case "list":
			opts.List = globals.ToUint64(value[0])
		case "listcount":
			opts.ListCount = globals.ToUint64(value[0])
		}
	}
	opts.Globals = *globals.FromRequest(w, r)

	return opts
}
