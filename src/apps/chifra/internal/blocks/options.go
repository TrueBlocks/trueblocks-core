// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package blocksPkg

import (
	"fmt"
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/blockRange"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

type BlocksOptions struct {
	Blocks     []string
	BlockIds   []blockRange.Identifier
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
	Globals    globals.GlobalOptions
	BadFlag    error
}

var blocksCmdLineOptions BlocksOptions

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
	logger.TestLog(opts.ListCount != 0, "ListCount: ", opts.ListCount)
	opts.Globals.TestLog()
}

func (opts *BlocksOptions) ToCmdLine() string {
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
	options += fmt.Sprintf("%s", "") // silence go compiler for auto gen
	return options
}

func BlocksFinishParseApi(w http.ResponseWriter, r *http.Request) *BlocksOptions {
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

func BlocksFinishParse(args []string) *BlocksOptions {
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
