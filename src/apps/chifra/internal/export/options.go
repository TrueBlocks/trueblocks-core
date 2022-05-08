// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package exportPkg

import (
	"fmt"
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient/ens"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

type ExportOptions struct {
	Addrs       []string
	Topics      []string
	Fourbytes   []string
	Appearances bool
	Receipts    bool
	Logs        bool
	Traces      bool
	Statements  bool
	Neighbors   bool
	Accounting  bool
	Articulate  bool
	Cache       bool
	CacheTraces bool
	Count       bool
	FirstRecord uint64
	MaxRecords  uint64
	Relevant    bool
	Emitter     []string
	Topic       []string
	Asset       []string
	Factory     bool
	Staging     bool
	Unripe      bool
	Load        string
	Reversed    bool
	SkipDdos    bool
	MaxTraces   uint64
	FirstBlock  uint64
	LastBlock   uint64
	Globals     globals.GlobalOptions
	BadFlag     error
}

var exportCmdLineOptions ExportOptions

func (opts *ExportOptions) TestLog() {
	logger.TestLog(len(opts.Addrs) > 0, "Addrs: ", opts.Addrs)
	logger.TestLog(len(opts.Topics) > 0, "Topics: ", opts.Topics)
	logger.TestLog(len(opts.Fourbytes) > 0, "Fourbytes: ", opts.Fourbytes)
	logger.TestLog(opts.Appearances, "Appearances: ", opts.Appearances)
	logger.TestLog(opts.Receipts, "Receipts: ", opts.Receipts)
	logger.TestLog(opts.Logs, "Logs: ", opts.Logs)
	logger.TestLog(opts.Traces, "Traces: ", opts.Traces)
	logger.TestLog(opts.Statements, "Statements: ", opts.Statements)
	logger.TestLog(opts.Neighbors, "Neighbors: ", opts.Neighbors)
	logger.TestLog(opts.Accounting, "Accounting: ", opts.Accounting)
	logger.TestLog(opts.Articulate, "Articulate: ", opts.Articulate)
	logger.TestLog(opts.Cache, "Cache: ", opts.Cache)
	logger.TestLog(opts.CacheTraces, "CacheTraces: ", opts.CacheTraces)
	logger.TestLog(opts.Count, "Count: ", opts.Count)
	logger.TestLog(opts.FirstRecord != 0, "FirstRecord: ", opts.FirstRecord)
	logger.TestLog(opts.MaxRecords != 250, "MaxRecords: ", opts.MaxRecords)
	logger.TestLog(opts.Relevant, "Relevant: ", opts.Relevant)
	logger.TestLog(len(opts.Emitter) > 0, "Emitter: ", opts.Emitter)
	logger.TestLog(len(opts.Topic) > 0, "Topic: ", opts.Topic)
	logger.TestLog(len(opts.Asset) > 0, "Asset: ", opts.Asset)
	logger.TestLog(opts.Factory, "Factory: ", opts.Factory)
	logger.TestLog(opts.Staging, "Staging: ", opts.Staging)
	logger.TestLog(opts.Unripe, "Unripe: ", opts.Unripe)
	logger.TestLog(len(opts.Load) > 0, "Load: ", opts.Load)
	logger.TestLog(opts.Reversed, "Reversed: ", opts.Reversed)
	logger.TestLog(opts.SkipDdos, "SkipDdos: ", opts.SkipDdos)
	logger.TestLog(opts.MaxTraces != 250, "MaxTraces: ", opts.MaxTraces)
	logger.TestLog(opts.FirstBlock != 0, "FirstBlock: ", opts.FirstBlock)
	logger.TestLog(opts.LastBlock != 0 && opts.LastBlock != utils.NOPOS, "LastBlock: ", opts.LastBlock)
	opts.Globals.TestLog()
}

func (opts *ExportOptions) ToCmdLine() string {
	options := ""
	if opts.Appearances {
		options += " --appearances"
	}
	if opts.Receipts {
		options += " --receipts"
	}
	if opts.Logs {
		options += " --logs"
	}
	if opts.Traces {
		options += " --traces"
	}
	if opts.Statements {
		options += " --statements"
	}
	if opts.Neighbors {
		options += " --neighbors"
	}
	if opts.Accounting {
		options += " --accounting"
	}
	if opts.Articulate {
		options += " --articulate"
	}
	if opts.Cache {
		options += " --cache"
	}
	if opts.CacheTraces {
		options += " --cache_traces"
	}
	if opts.Count {
		options += " --count"
	}
	if opts.FirstRecord != 0 {
		options += (" --first_record " + fmt.Sprintf("%d", opts.FirstRecord))
	}
	if opts.MaxRecords != 250 {
		options += (" --max_records " + fmt.Sprintf("%d", opts.MaxRecords))
	}
	if opts.Relevant {
		options += " --relevant"
	}
	for _, emitter := range opts.Emitter {
		options += " --emitter " + emitter
	}
	for _, topic := range opts.Topic {
		options += " --topic " + topic
	}
	for _, asset := range opts.Asset {
		options += " --asset " + asset
	}
	if opts.Factory {
		options += " --factory"
	}
	if len(opts.Load) > 0 {
		options += " --load " + opts.Load
	}
	if opts.Reversed {
		options += " --reversed"
	}
	if opts.FirstBlock != 0 {
		options += (" --first_block " + fmt.Sprintf("%d", opts.FirstBlock))
	}
	if opts.LastBlock != 0 && opts.LastBlock != utils.NOPOS {
		options += (" --last_block " + fmt.Sprintf("%d", opts.LastBlock))
	}
	options += " " + strings.Join(opts.Addrs, " ")
	options += " " + strings.Join(opts.Topics, " ")
	options += " " + strings.Join(opts.Fourbytes, " ")
	options += fmt.Sprintf("%s", "") // silence go compiler for auto gen
	return options
}

func FromRequest(w http.ResponseWriter, r *http.Request) *ExportOptions {
	opts := &ExportOptions{}
	opts.FirstRecord = 0
	opts.MaxRecords = 250
	opts.MaxTraces = 250
	opts.FirstBlock = 0
	opts.LastBlock = utils.NOPOS
	for key, value := range r.URL.Query() {
		switch key {
		case "addrs":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Addrs = append(opts.Addrs, s...)
			}
		case "topics":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Topics = append(opts.Topics, s...)
			}
		case "fourbytes":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Fourbytes = append(opts.Fourbytes, s...)
			}
		case "appearances":
			opts.Appearances = true
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
		case "cacheTraces":
			opts.CacheTraces = true
		case "count":
			opts.Count = true
		case "firstRecord":
			opts.FirstRecord = globals.ToUint64(value[0])
		case "maxRecords":
			opts.MaxRecords = globals.ToUint64(value[0])
		case "relevant":
			opts.Relevant = true
		case "emitter":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Emitter = append(opts.Emitter, s...)
			}
		case "topic":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Topic = append(opts.Topic, s...)
			}
		case "asset":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Asset = append(opts.Asset, s...)
			}
		case "factory":
			opts.Factory = true
		case "staging":
			opts.Staging = true
		case "unripe":
			opts.Unripe = true
		case "load":
			opts.Load = value[0]
		case "reversed":
			opts.Reversed = true
		case "skipDdos":
			opts.SkipDdos = true
		case "maxTraces":
			opts.MaxTraces = globals.ToUint64(value[0])
		case "firstBlock":
			opts.FirstBlock = globals.ToUint64(value[0])
		case "lastBlock":
			opts.LastBlock = globals.ToUint64(value[0])
		default:
			if !globals.IsGlobalOption(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "export")
				return opts
			}
		}
	}
	opts.Globals = *globals.FromRequest(w, r)
	// EXISTING_CODE
	opts.Addrs = ens.ConvertEns(opts.Globals.Chain, opts.Addrs)
	opts.Emitter = ens.ConvertEns(opts.Globals.Chain, opts.Emitter)
	opts.Asset = ens.ConvertEns(opts.Globals.Chain, opts.Asset)
	// EXISTING_CODE

	return opts
}

func ExportFinishParse(args []string) *ExportOptions {
	opts := GetOptions()
	opts.Globals.FinishParse(args)
	// EXISTING_CODE
	for _, arg := range args {
		if validate.IsValidTopic(arg) {
			opts.Topics = append(opts.Topics, arg)
		} else if validate.IsValidFourByte(arg) {
			opts.Fourbytes = append(opts.Fourbytes, arg)
		} else {
			opts.Addrs = append(opts.Addrs, arg)
		}
	}
	opts.Addrs = ens.ConvertEns(opts.Globals.Chain, opts.Addrs)
	opts.Emitter = ens.ConvertEns(opts.Globals.Chain, opts.Emitter)
	opts.Asset = ens.ConvertEns(opts.Globals.Chain, opts.Asset)
	// EXISTING_CODE
	return opts
}

func GetOptions() *ExportOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &exportCmdLineOptions
}
