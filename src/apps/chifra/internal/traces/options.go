// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package tracesPkg

import (
	"fmt"
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

type TracesOptions struct {
	Transactions []string
	Articulate   bool
	Filter       string
	Statediff    bool
	Count        bool
	SkipDdos     bool
	Max          uint64
	Globals      globals.GlobalOptions
	BadFlag      error
}

var tracesCmdLineOptions TracesOptions

func (opts *TracesOptions) TestLog() {
	logger.TestLog(len(opts.Transactions) > 0, "Transactions: ", opts.Transactions)
	logger.TestLog(opts.Articulate, "Articulate: ", opts.Articulate)
	logger.TestLog(len(opts.Filter) > 0, "Filter: ", opts.Filter)
	logger.TestLog(opts.Statediff, "Statediff: ", opts.Statediff)
	logger.TestLog(opts.Count, "Count: ", opts.Count)
	logger.TestLog(opts.SkipDdos, "SkipDdos: ", opts.SkipDdos)
	logger.TestLog(opts.Max != 250, "Max: ", opts.Max)
	opts.Globals.TestLog()
}

func (opts *TracesOptions) ToCmdLine() string {
	options := ""
	if opts.Articulate {
		options += " --articulate"
	}
	if len(opts.Filter) > 0 {
		options += " --filter " + opts.Filter
	}
	if opts.Statediff {
		options += " --statediff"
	}
	if opts.Count {
		options += " --count"
	}
	if opts.SkipDdos {
		options += " --skip_ddos"
	}
	if opts.Max != 250 {
		options += (" --max " + fmt.Sprintf("%d", opts.Max))
	}
	options += " " + strings.Join(opts.Transactions, " ")
	options += fmt.Sprintf("%s", "") // silence go compiler for auto gen
	return options
}

func TracesFinishParseApi(w http.ResponseWriter, r *http.Request) *TracesOptions {
	opts := &TracesOptions{}
	opts.Max = 250
	for key, value := range r.URL.Query() {
		switch key {
		case "transactions":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Transactions = append(opts.Transactions, s...)
			}
		case "articulate":
			opts.Articulate = true
		case "filter":
			opts.Filter = value[0]
		case "statediff":
			opts.Statediff = true
		case "count":
			opts.Count = true
		case "skipDdos":
			opts.SkipDdos = true
		case "max":
			opts.Max = globals.ToUint64(value[0])
		default:
			if !globals.IsGlobalOption(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "traces")
				return opts
			}
		}
	}
	opts.Globals = *globals.GlobalsFinishParseApi(w, r)
	// EXISTING_CODE
	// EXISTING_CODE

	return opts
}

func TracesFinishParse(args []string) *TracesOptions {
	opts := GetOptions()
	opts.Globals.FinishParse(args)
	defFmt := "txt"
	// EXISTING_CODE
	opts.Transactions = args
	// EXISTING_CODE
	if len(opts.Globals.Format) == 0 || opts.Globals.Format == "none" {
		opts.Globals.Format = defFmt
	}
	return opts
}

func GetOptions() *TracesOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &tracesCmdLineOptions
}
