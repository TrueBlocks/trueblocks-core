package exportPkg

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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

type ExportOptionsType struct {
	Addrs        []string
	Topics       []string
	Fourbytes    []string
	Appearances  bool
	Transactions bool
	Receipts     bool
	Logs         bool
	Traces       bool
	Statements   bool
	Neighbors    bool
	Accounting   bool
	Articulate   bool
	Cache        bool
	CacheTraces  bool
	Factory      bool
	Count        bool
	FirstRecord  uint64
	MaxRecords   uint64
	Relevant     bool
	Emitter      []string
	Topic        []string
	Clean        bool
	Freshen      bool
	Staging      bool
	Unripe       bool
	Load         string
	Reversed     bool
	ByDate       bool
	SummarizeBy  string
	SkipDdos     bool
	MaxTraces    uint64
	FirstBlock   uint64
	LastBlock    uint64
	Globals      root.GlobalOptionsType
}

var Options ExportOptionsType

func (opts *ExportOptionsType) TestLog() {
	logger.TestLog(len(opts.Addrs) > 0, "Addrs: ", opts.Addrs)
	logger.TestLog(len(opts.Topics) > 0, "Topics: ", opts.Topics)
	logger.TestLog(len(opts.Fourbytes) > 0, "Fourbytes: ", opts.Fourbytes)
	logger.TestLog(opts.Appearances, "Appearances: ", opts.Appearances)
	logger.TestLog(opts.Transactions, "Transactions: ", opts.Transactions)
	logger.TestLog(opts.Receipts, "Receipts: ", opts.Receipts)
	logger.TestLog(opts.Logs, "Logs: ", opts.Logs)
	logger.TestLog(opts.Traces, "Traces: ", opts.Traces)
	logger.TestLog(opts.Statements, "Statements: ", opts.Statements)
	logger.TestLog(opts.Neighbors, "Neighbors: ", opts.Neighbors)
	logger.TestLog(opts.Accounting, "Accounting: ", opts.Accounting)
	logger.TestLog(opts.Articulate, "Articulate: ", opts.Articulate)
	logger.TestLog(opts.Cache, "Cache: ", opts.Cache)
	logger.TestLog(opts.CacheTraces, "CacheTraces: ", opts.CacheTraces)
	logger.TestLog(opts.Factory, "Factory: ", opts.Factory)
	logger.TestLog(opts.Count, "Count: ", opts.Count)
	logger.TestLog(opts.FirstRecord != 0, "FirstRecord: ", opts.FirstRecord)
	logger.TestLog(opts.MaxRecords != 250, "MaxRecords: ", opts.MaxRecords)
	logger.TestLog(opts.Relevant, "Relevant: ", opts.Relevant)
	logger.TestLog(len(opts.Emitter) > 0, "Emitter: ", opts.Emitter)
	logger.TestLog(len(opts.Topic) > 0, "Topic: ", opts.Topic)
	logger.TestLog(opts.Clean, "Clean: ", opts.Clean)
	logger.TestLog(opts.Freshen, "Freshen: ", opts.Freshen)
	logger.TestLog(opts.Staging, "Staging: ", opts.Staging)
	logger.TestLog(opts.Unripe, "Unripe: ", opts.Unripe)
	logger.TestLog(len(opts.Load) > 0, "Load: ", opts.Load)
	logger.TestLog(opts.Reversed, "Reversed: ", opts.Reversed)
	logger.TestLog(opts.ByDate, "ByDate: ", opts.ByDate)
	logger.TestLog(len(opts.SummarizeBy) > 0, "SummarizeBy: ", opts.SummarizeBy)
	logger.TestLog(opts.SkipDdos, "SkipDdos: ", opts.SkipDdos)
	logger.TestLog(opts.MaxTraces != 250, "MaxTraces: ", opts.MaxTraces)
	logger.TestLog(opts.FirstBlock != 0, "FirstBlock: ", opts.FirstBlock)
	logger.TestLog(opts.LastBlock != utils.NOPOS, "LastBlock: ", opts.LastBlock)
	opts.Globals.TestLog()
}

func FromRequest(r *http.Request) *ExportOptionsType {
	opts := &ExportOptionsType{}
	for key, value := range r.URL.Query() {
		switch key {
		case "addrs":
			opts.Addrs = append(opts.Addrs, value...)
		case "topics":
			opts.Topics = append(opts.Topics, value...)
		case "fourbytes":
			opts.Fourbytes = append(opts.Fourbytes, value...)
		case "appearances":
			opts.Appearances = true
		case "transactions":
			opts.Transactions = true
		case "receipts":
			opts.Receipts = true
		case "logs":
			opts.Logs = true
		case "traces":
			opts.Traces = true
		case "statements":
			opts.Statements = true
		case "neighbors":
			opts.Neighbors = true
		case "accounting":
			opts.Accounting = true
		case "articulate":
			opts.Articulate = true
		case "cache":
			opts.Cache = true
		case "cachetraces":
			opts.CacheTraces = true
		case "factory":
			opts.Factory = true
		case "count":
			opts.Count = true
		case "firstrecord":
			opts.FirstRecord = root.ToUint64(value[0])
		case "maxrecords":
			opts.MaxRecords = root.ToUint64(value[0])
		case "relevant":
			opts.Relevant = true
		case "emitter":
			opts.Emitter = append(opts.Emitter, value...)
		case "topic":
			opts.Topic = append(opts.Topic, value...)
		case "clean":
			opts.Clean = true
		case "freshen":
			opts.Freshen = true
		case "staging":
			opts.Staging = true
		case "unripe":
			opts.Unripe = true
		case "load":
			opts.Load = value[0]
		case "reversed":
			opts.Reversed = true
		case "bydate":
			opts.ByDate = true
		case "summarizeby":
			opts.SummarizeBy = value[0]
		case "skipddos":
			opts.SkipDdos = true
		case "maxtraces":
			opts.MaxTraces = root.ToUint64(value[0])
		case "firstblock":
			opts.FirstBlock = root.ToUint64(value[0])
		case "lastblock":
			opts.LastBlock = root.ToUint64(value[0])
		}
	}
	opts.Globals = *root.FromRequest(r)

	return opts
}
