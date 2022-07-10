// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package exportPkg

import (
	"encoding/json"
	"fmt"
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient/ens"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

// ExportOptions provides all command options for the chifra export command.
type ExportOptions struct {
	// one or more addresses (0x...) to export
	Addrs []string `json:"addrs,omitempty"`
	// filter by one or more log topics (only for --logs option)
	Topics []string `json:"topics,omitempty"`
	// filter by one or more fourbytes (only for transactions and trace options)
	Fourbytes []string `json:"fourbytes,omitempty"`
	// export a list of appearances
	Appearances bool `json:"appearances,omitempty"`
	// export receipts instead of transactional data
	Receipts bool `json:"receipts,omitempty"`
	// export logs instead of transactional data
	Logs bool `json:"logs,omitempty"`
	// export traces instead of transactional data
	Traces bool `json:"traces,omitempty"`
	// export reconciliations instead of transactional data (requires --accounting option)
	Statements bool `json:"statements,omitempty"`
	// export the neighbors of the given address
	Neighbors bool `json:"neighbors,omitempty"`
	// attach accounting records to the exported data (applies to transactions export only)
	Accounting bool `json:"accounting,omitempty"`
	// articulate transactions, traces, logs, and outputs
	Articulate bool `json:"articulate,omitempty"`
	// write transactions to the cache (see notes)
	Cache bool `json:"cache,omitempty"`
	// write traces to the cache (see notes)
	CacheTraces bool `json:"cacheTraces,omitempty"`
	// only available for --appearances mode, if present, return only the number of records
	Count bool `json:"count,omitempty"`
	// the first record to process
	FirstRecord uint64 `json:"firstRecord,omitempty"`
	// the maximum number of records to process before reporting
	MaxRecords uint64 `json:"maxRecords,omitempty"`
	// for log and accounting export only, export only logs relevant to one of the given export addresses
	Relevant bool `json:"relevant,omitempty"`
	// for log export only, export only logs if emitted by one of these address(es)
	Emitter []string `json:"emitter,omitempty"`
	// for log export only, export only logs with this topic(s)
	Topic []string `json:"topic,omitempty"`
	// for the statements option only, export only reconciliations for this asset
	Asset []string `json:"asset,omitempty"`
	// scan for contract creations from the given address(es) and report address of those contracts
	Factory bool `json:"factory,omitempty"`
	// export transactions labeled staging (i.e. older than 28 blocks but not yet consolidated)
	Staging bool `json:"staging,omitempty"`
	// export transactions labeled upripe (i.e. less than 28 blocks old)
	Unripe bool `json:"unripe,omitempty"`
	// a comma separated list of dynamic traversers to load
	Load string `json:"load,omitempty"`
	// produce results in reverse chronological order
	Reversed bool `json:"reversed,omitempty"`
	// first block to process (inclusive)
	FirstBlock uint64 `json:"firstBlock,omitempty"`
	// last block to process (inclusive)
	LastBlock uint64 `json:"lastBlock,omitempty"`
	// the global options
	Globals globals.GlobalOptions `json:"globals,omitempty"`
	// an error flag if needed
	BadFlag error `json:"badFlag,omitempty"`
}

var exportCmdLineOptions ExportOptions

// testLog is used only during testing to export the options for this test case.
func (opts *ExportOptions) testLog() {
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
	logger.TestLog(opts.FirstBlock != 0, "FirstBlock: ", opts.FirstBlock)
	logger.TestLog(opts.LastBlock != 0 && opts.LastBlock != utils.NOPOS, "LastBlock: ", opts.LastBlock)
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *ExportOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "\t")
	return string(b)
}

// getEnvStr allows for custom environment strings when calling to the system (helps debugging).
func (opts *ExportOptions) getEnvStr() []string {
	envStr := []string{}
	// EXISTING_CODE
	// EXISTING_CODE
	return envStr
}

// toCmdLine converts the option to a command line for calling out to the system.
func (opts *ExportOptions) toCmdLine() string {
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
	// EXISTING_CODE
	// EXISTING_CODE
	options += fmt.Sprintf("%s", "") // silence go compiler for auto gen
	return options
}

// exportFinishParseApi finishes the parsing for server invocations. Returns a new ExportOptions.
func exportFinishParseApi(w http.ResponseWriter, r *http.Request) *ExportOptions {
	opts := &ExportOptions{}
	opts.FirstRecord = 0
	opts.MaxRecords = 250
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
	opts.Globals = *globals.GlobalsFinishParseApi(w, r)
	// EXISTING_CODE
	opts.Addrs = ens.ConvertEns(opts.Globals.Chain, opts.Addrs)
	opts.Emitter = ens.ConvertEns(opts.Globals.Chain, opts.Emitter)
	opts.Asset = ens.ConvertEns(opts.Globals.Chain, opts.Asset)
	// EXISTING_CODE

	return opts
}

// exportFinishParse finishes the parsing for command line invocations. Returns a new ExportOptions.
func exportFinishParse(args []string) *ExportOptions {
	opts := GetOptions()
	opts.Globals.FinishParse(args)
	defFmt := "txt"
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
	if len(opts.Globals.Format) == 0 || opts.Globals.Format == "none" {
		opts.Globals.Format = defFmt
	}
	return opts
}

func GetOptions() *ExportOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &exportCmdLineOptions
}
