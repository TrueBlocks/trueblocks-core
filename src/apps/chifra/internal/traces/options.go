// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package tracesPkg

import (
	"encoding/json"
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

// TracesOptions provides all command options for the chifra traces command.
type TracesOptions struct {
	Transactions   []string                 `json:"transactions,omitempty"`   // A space-separated list of one or more transaction identifiers
	TransactionIds []identifiers.Identifier `json:"transactionIds,omitempty"` // Transaction identifiers
	Articulate     bool                     `json:"articulate,omitempty"`     // Articulate the retrieved data if ABIs can be found
	Filter         string                   `json:"filter,omitempty"`         // Call the node's trace_filter routine with bang-separated filter
	Count          bool                     `json:"count,omitempty"`          // Show the number of traces for the transaction only (fast)
	Globals        globals.GlobalOptions    `json:"globals,omitempty"`        // The global options
	BadFlag        error                    `json:"badFlag,omitempty"`        // An error flag if needed
	// EXISTING_CODE
	// EXISTING_CODE
}

var defaultTracesOptions = TracesOptions{}

// testLog is used only during testing to export the options for this test case.
func (opts *TracesOptions) testLog() {
	logger.TestLog(len(opts.Transactions) > 0, "Transactions: ", opts.Transactions)
	logger.TestLog(opts.Articulate, "Articulate: ", opts.Articulate)
	logger.TestLog(len(opts.Filter) > 0, "Filter: ", opts.Filter)
	logger.TestLog(opts.Count, "Count: ", opts.Count)
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *TracesOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "  ")
	return string(b)
}

// tracesFinishParseApi finishes the parsing for server invocations. Returns a new TracesOptions.
func tracesFinishParseApi(w http.ResponseWriter, r *http.Request) *TracesOptions {
	copy := defaultTracesOptions
	opts := &copy
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
		case "count":
			opts.Count = true
		default:
			if !globals.IsGlobalOption(copy.Globals.Caps, key) {
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

// tracesFinishParse finishes the parsing for command line invocations. Returns a new TracesOptions.
func tracesFinishParse(args []string) *TracesOptions {
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
	return &defaultTracesOptions
}

func ResetOptions() {
	// We want to keep writer between command file calls
	w := GetOptions().Globals.Writer
	defaultTracesOptions = TracesOptions{}
	globals.SetDefaults(&defaultTracesOptions.Globals)
	defaultTracesOptions.Globals.Writer = w
	defaultTracesOptions.Globals.Caps = caps.None // Additional global caps for use with --file option
	// EXISTING_CODE
	defaultTracesOptions.Globals.Caps = defaultTracesOptions.Globals.Caps.Add(caps.Caching)
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
