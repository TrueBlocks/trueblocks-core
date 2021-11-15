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
	"fmt"
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/cmd/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

type ExportOptions struct {
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
	Globals      globals.GlobalOptionsType
}

func (opts *ExportOptions) TestLog() {
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

func (opts *ExportOptions) ToDashStr() string {
	options := ""
	if Options.Appearances {
		options += " --appearances"
	}
	if Options.Receipts {
		options += " --receipts"
	}
	if Options.Statements {
		options += " --statements"
	}
	if Options.Logs {
		options += " --logs"
	}
	if Options.Traces {
		options += " --traces"
	}
	if Options.Accounting {
		options += " --accounting"
	}
	if Options.Articulate {
		options += " --articulate"
	}
	if Options.Cache {
		options += " --cache"
	}
	if Options.CacheTraces {
		options += " --cache_traces"
	}
	if Options.Neighbors {
		options += " --neighbors"
	}
	if Options.Factory {
		options += " --factory"
	}
	for _, e := range Options.Emitter {
		options += " --emitter " + e
	}
	for _, t := range Options.Topic {
		options += " --topic " + t
	}
	if Options.Relevant {
		options += " --relevant"
	}
	if Options.Count {
		options += " --count"
	}
	if Options.FirstRecord > 0 {
		options += " --first_record " + fmt.Sprintf("%d", Options.FirstRecord)
	}
	if Options.MaxRecords > 0 && Options.MaxRecords != 250 {
		options += " --max_records " + fmt.Sprintf("%d", Options.MaxRecords)
	}
	if Options.Clean {
		options += " --clean"
	}
	if Options.Freshen {
		options += " --freshen"
	}
	if Options.Staging {
		options += " --staging"
	}
	if Options.Unripe {
		options += " --unripe"
	}
	if len(Options.Load) > 0 {
		options += " --load " + Options.Load
	}
	if Options.Reversed {
		options += " --reversed"
	}
	if Options.ByDate {
		options += " --by_date"
	}
	if len(Options.SummarizeBy) > 0 {
		options += " --summarize_by " + Options.SummarizeBy
	}
	if Options.SkipDdos {
		options += " --skip_ddos"
	}
	if Options.MaxTraces != 250 {
		options += " --max_traces " + fmt.Sprintf("%d", Options.MaxTraces)
	}
	if Options.FirstBlock > 0 {
		options += " --first_block " + fmt.Sprintf("%d", Options.FirstBlock)
	}
	if Options.LastBlock > 0 {
		options += " --last_block " + fmt.Sprintf("%d", Options.LastBlock)
	}
	options += " " + strings.Join(opts.Addrs, " ")
	return options
}

func FromRequest(w http.ResponseWriter, r *http.Request) *ExportOptions {
	opts := &ExportOptions{}
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
		case "cache_traces":
			opts.CacheTraces = true
		case "factory":
			opts.Factory = true
		case "count":
			opts.Count = true
		case "first_record":
			opts.FirstRecord = globals.ToUint64(value[0])
		case "max_records":
			opts.MaxRecords = globals.ToUint64(value[0])
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
		case "by_date":
			opts.ByDate = true
		case "summarize_by":
			opts.SummarizeBy = value[0]
		case "skip_ddos":
			opts.SkipDdos = true
		case "max_traces":
			opts.MaxTraces = globals.ToUint64(value[0])
		case "first_block":
			opts.FirstBlock = globals.ToUint64(value[0])
		case "last_block":
			opts.LastBlock = globals.ToUint64(value[0])
		}
	}
	opts.Globals = *globals.FromRequest(w, r)

	return opts
}
