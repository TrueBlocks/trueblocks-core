// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package tracesPkg

import (
	"encoding/json"
	"io"
	"net/http"
	"net/url"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

// TracesOptions provides all command options for the chifra traces command.
type TracesOptions struct {
	Transactions   []string                 `json:"transactions,omitempty"`   // A space-separated list of one or more transaction identifiers
	TransactionIds []identifiers.Identifier `json:"transactionIds,omitempty"` // Transaction identifiers
	Articulate     bool                     `json:"articulate,omitempty"`     // Articulate the retrieved data if ABIs can be found
	Filter         string                   `json:"filter,omitempty"`         // Call the node's trace_filter routine with bang-separated filter
	Count          bool                     `json:"count,omitempty"`          // Display only the number of traces for the transaction (fast)
	Globals        globals.GlobalOptions    `json:"globals,omitempty"`        // The global options
	Conn           *rpc.Connection          `json:"conn,omitempty"`           // The connection to the RPC server
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
	opts.Conn.TestLog(opts.getCaches())
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *TracesOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "  ")
	return string(b)
}

// tracesFinishParseApi finishes the parsing for server invocations. Returns a new TracesOptions.
func tracesFinishParseApi(w http.ResponseWriter, r *http.Request) *TracesOptions {
	values := r.URL.Query()
	if r.Header.Get("User-Agent") == "testRunner" {
		values.Set("testRunner", "true")
	}
	return TracesFinishParseInternal(w, values)
}

func TracesFinishParseInternal(w io.Writer, values url.Values) *TracesOptions {
	copy := defaultTracesOptions
	opts := &copy
	for key, value := range values {
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
			if !copy.Globals.Caps.HasKey(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "traces")
			}
		}
	}
	opts.Conn = opts.Globals.FinishParseApi(w, values, opts.getCaches())

	// EXISTING_CODE
	// EXISTING_CODE

	return opts
}

// tracesFinishParse finishes the parsing for command line invocations. Returns a new TracesOptions.
func tracesFinishParse(args []string) *TracesOptions {
	// remove duplicates from args if any (not needed in api mode because the server does it).
	dedup := map[string]int{}
	if len(args) > 0 {
		tmp := []string{}
		for _, arg := range args {
			if value := dedup[arg]; value == 0 {
				tmp = append(tmp, arg)
			}
			dedup[arg]++
		}
		args = tmp
	}

	defFmt := "txt"
	opts := GetOptions()
	opts.Conn = opts.Globals.FinishParse(args, opts.getCaches())

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

func ResetOptions(testMode bool) {
	// We want to keep writer between command file calls
	w := GetOptions().Globals.Writer
	defaultTracesOptions = TracesOptions{}
	globals.SetDefaults(&defaultTracesOptions.Globals)
	defaultTracesOptions.Globals.TestMode = testMode
	defaultTracesOptions.Globals.Writer = w
	capabilities := caps.Default // Additional global caps for chifra traces
	// EXISTING_CODE
	capabilities = capabilities.Add(caps.Caching)
	capabilities = capabilities.Add(caps.Ether)
	capabilities = capabilities.Add(caps.Raw)
	// EXISTING_CODE
	defaultTracesOptions.Globals.Caps = capabilities
}

func (opts *TracesOptions) getCaches() (m map[string]bool) {
	// EXISTING_CODE
	m = map[string]bool{
		"transactions": true,
		"traces":       true,
	}
	// EXISTING_CODE
	return
}

// EXISTING_CODE
// EXISTING_CODE

