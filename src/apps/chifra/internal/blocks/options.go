// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package blocksPkg

import (
	"encoding/json"
	"fmt"
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

// BlocksOptions provides all command options for the chifra blocks command.
type BlocksOptions struct {
	// a space-separated list of one or more block identifiers
	Blocks []string `json:"blocks,omitempty"`
	// block identifiers
	BlockIds []identifiers.Identifier `json:"blockIds,omitempty"`
	// display only transaction hashes, default is to display full transaction detail
	Hashes bool `json:"hashes,omitempty"`
	// display uncle blocks (if any) instead of the requested block
	Uncles bool `json:"uncles,omitempty"`
	// export the traces from the block as opposed to the block data
	Trace bool `json:"trace,omitempty"`
	// display a list of uniq address appearances in the block
	Apps bool `json:"apps,omitempty"`
	// display a list of uniq address appearances per transaction
	Uniq bool `json:"uniq,omitempty"`
	// display only the logs found in the block(s)
	Logs bool `json:"logs,omitempty"`
	// for the --logs option only, filter logs to show only those logs emitted by the given address(es)
	Emitter []string `json:"emitter,omitempty"`
	// for the --logs option only, filter logs to show only those with this topic(s)
	Topic []string `json:"topic,omitempty"`
	// for the --logs option only, articulate the retrieved data if ABIs can be found
	Articulate bool `json:"articulate,omitempty"`
	// for the --logs option only, allow for block ranges larger than 500
	BigRange uint64 `json:"bigRange,omitempty"`
	// display the number of the lists of appearances for --addrs or --uniq
	Count bool `json:"count,omitempty"`
	// force a write of the block to the cache
	Cache bool `json:"cache,omitempty"`
	// summary list of blocks running backwards from latest block minus num
	List uint64 `json:"list,omitempty"`
	// the number of blocks to report for --list option
	ListCount uint64 `json:"listCount,omitempty"`
	// the global options
	Globals globals.GlobalOptions `json:"globals,omitempty"`
	// an error flag if needed
	BadFlag error `json:"badFlag,omitempty"`
}

var blocksCmdLineOptions BlocksOptions

// testLog is used only during testing to export the options for this test case.
func (opts *BlocksOptions) testLog() {
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
	logger.TestLog(opts.ListCount != 0, "ListCount: ", opts.ListCount)
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *BlocksOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "\t")
	return string(b)
}

// getEnvStr allows for custom environment strings when calling to the system (helps debugging).
func (opts *BlocksOptions) getEnvStr() []string {
	envStr := []string{}
	// EXISTING_CODE
	// EXISTING_CODE
	return envStr
}

// toCmdLine converts the option to a command line for calling out to the system.
func (opts *BlocksOptions) toCmdLine() string {
	options := ""
	if opts.Hashes {
		options += " --hashes"
	}
	if opts.Uncles {
		options += " --uncles"
	}
	if opts.Trace {
		options += " --trace"
	}
	if opts.Apps {
		options += " --apps"
	}
	if opts.Uniq {
		options += " --uniq"
	}
	if opts.Logs {
		options += " --logs"
	}
	for _, emitter := range opts.Emitter {
		options += " --emitter " + emitter
	}
	for _, topic := range opts.Topic {
		options += " --topic " + topic
	}
	if opts.Articulate {
		options += " --articulate"
	}
	if opts.BigRange != 500 {
		options += (" --big_range " + fmt.Sprintf("%d", opts.BigRange))
	}
	if opts.Count {
		options += " --count"
	}
	if opts.Cache {
		options += " --cache"
	}
	if opts.List != 0 {
		options += (" --list " + fmt.Sprintf("%d", opts.List))
	}
	if opts.ListCount != 0 {
		options += (" --list_count " + fmt.Sprintf("%d", opts.ListCount))
	}
	options += " " + strings.Join(opts.Blocks, " ")
	// EXISTING_CODE
	// EXISTING_CODE
	options += fmt.Sprintf("%s", "") // silence go compiler for auto gen
	return options
}

// blocksFinishParseApi finishes the parsing for server invocations. Returns a new BlocksOptions.
func blocksFinishParseApi(w http.ResponseWriter, r *http.Request) *BlocksOptions {
	opts := &BlocksOptions{}
	opts.BigRange = 500
	opts.List = 0
	opts.ListCount = 0
	for key, value := range r.URL.Query() {
		switch key {
		case "blocks":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Blocks = append(opts.Blocks, s...)
			}
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
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Emitter = append(opts.Emitter, s...)
			}
		case "topic":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Topic = append(opts.Topic, s...)
			}
		case "articulate":
			opts.Articulate = true
		case "bigRange":
			opts.BigRange = globals.ToUint64(value[0])
		case "count":
			opts.Count = true
		case "cache":
			opts.Cache = true
		case "list":
			opts.List = globals.ToUint64(value[0])
		case "listCount":
			opts.ListCount = globals.ToUint64(value[0])
		default:
			if !globals.IsGlobalOption(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "blocks")
				return opts
			}
		}
	}
	opts.Globals = *globals.GlobalsFinishParseApi(w, r)
	// EXISTING_CODE
	// EXISTING_CODE

	return opts
}

// blocksFinishParse finishes the parsing for command line invocations. Returns a new BlocksOptions.
func blocksFinishParse(args []string) *BlocksOptions {
	opts := GetOptions()
	opts.Globals.FinishParse(args)
	defFmt := "txt"
	// EXISTING_CODE
	if !opts.Uniq {
		defFmt = "json"
	}
	opts.Blocks = args
	// EXISTING_CODE
	if len(opts.Globals.Format) == 0 || opts.Globals.Format == "none" {
		opts.Globals.Format = defFmt
	}
	return opts
}

func GetOptions() *BlocksOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &blocksCmdLineOptions
}
