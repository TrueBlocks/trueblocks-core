// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package transactionsPkg

import (
	// EXISTING_CODE
	"encoding/json"
	"io"
	"net/http"
	"net/url"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/validate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/walk"
	// EXISTING_CODE
)

// TransactionsOptions provides all command options for the chifra transactions command.
type TransactionsOptions struct {
	Transactions   []string                 `json:"transactions,omitempty"`   // A space-separated list of one or more transaction identifiers
	TransactionIds []identifiers.Identifier `json:"transactionIds,omitempty"` // Transaction identifiers
	Articulate     bool                     `json:"articulate,omitempty"`     // Articulate the retrieved data if ABIs can be found
	Traces         bool                     `json:"traces,omitempty"`         // Include the transaction's traces in the results
	Uniq           bool                     `json:"uniq,omitempty"`           // Display a list of uniq addresses found in the transaction
	Flow           string                   `json:"flow,omitempty"`           // For the uniq option only, export only from or to (including trace from or to)
	Logs           bool                     `json:"logs,omitempty"`           // Display only the logs found in the transaction(s)
	Emitter        []string                 `json:"emitter,omitempty"`        // For the --logs option only, filter logs to show only those logs emitted by the given address(es)
	Topic          []string                 `json:"topic,omitempty"`          // For the --logs option only, filter logs to show only those with this topic(s)
	CacheTraces    bool                     `json:"cacheTraces,omitempty"`    // Force the transaction's traces into the cache
	Globals        globals.GlobalOptions    `json:"globals,omitempty"`        // The global options
	Conn           *rpc.Connection          `json:"conn,omitempty"`           // The connection to the RPC server
	BadFlag        error                    `json:"badFlag,omitempty"`        // An error flag if needed
	// EXISTING_CODE
	// EXISTING_CODE
}

var defaultTransactionsOptions = TransactionsOptions{}

// testLog is used only during testing to export the options for this test case.
func (opts *TransactionsOptions) testLog() {
	logger.TestLog(len(opts.Transactions) > 0, "Transactions: ", opts.Transactions)
	logger.TestLog(opts.Articulate, "Articulate: ", opts.Articulate)
	logger.TestLog(opts.Traces, "Traces: ", opts.Traces)
	logger.TestLog(opts.Uniq, "Uniq: ", opts.Uniq)
	logger.TestLog(len(opts.Flow) > 0, "Flow: ", opts.Flow)
	logger.TestLog(opts.Logs, "Logs: ", opts.Logs)
	logger.TestLog(len(opts.Emitter) > 0, "Emitter: ", opts.Emitter)
	logger.TestLog(len(opts.Topic) > 0, "Topic: ", opts.Topic)
	logger.TestLog(opts.CacheTraces, "CacheTraces: ", opts.CacheTraces)
	opts.Conn.TestLog()
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *TransactionsOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "  ")
	return string(b)
}

// transactionsFinishParseApi finishes the parsing for server invocations. Returns a new TransactionsOptions.
func transactionsFinishParseApi(w http.ResponseWriter, r *http.Request) *TransactionsOptions {
	values := r.URL.Query()
	if r.Header.Get("User-Agent") == "testRunner" {
		values.Set("testRunner", "true")
	}
	return TransactionsFinishParseInternal(w, values)
}

func TransactionsFinishParseInternal(w io.Writer, values url.Values) *TransactionsOptions {
	copy := defaultTransactionsOptions
	copy.Globals.Caps = getCaps()
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
		case "traces":
			opts.Traces = true
		case "uniq":
			opts.Uniq = true
		case "flow":
			opts.Flow = value[0]
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
		case "cacheTraces":
			opts.CacheTraces = true
		default:
			if !copy.Globals.Caps.HasKey(key) {
				err := validate.Usage("Invalid key ({0}) in {1} route.", key, "transactions")
				if opts.BadFlag == nil || opts.BadFlag.Error() > err.Error() {
					opts.BadFlag = err
				}
			}
		}
	}
	opts.Conn = opts.Globals.FinishParseApi(w, values, opts.getCaches())

	// EXISTING_CODE
	// EXISTING_CODE
	opts.Emitter, _ = opts.Conn.GetEnsAddresses(opts.Emitter)

	return opts
}

// transactionsFinishParse finishes the parsing for command line invocations. Returns a new TransactionsOptions.
func transactionsFinishParse(args []string) *TransactionsOptions {
	// remove duplicates from args if any (not needed in api mode because the server does it).
	dedup := map[string]int{}
	if len(args) > 0 {
		tmp := []string{}
		for _, arg := range args {
			if cnt := dedup[arg]; cnt == 0 {
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
	opts.Emitter, _ = opts.Conn.GetEnsAddresses(opts.Emitter)
	if len(opts.Globals.Format) == 0 || opts.Globals.Format == "none" {
		opts.Globals.Format = defFmt
	}

	return opts
}

func GetOptions() *TransactionsOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &defaultTransactionsOptions
}

func getCaps() caps.Capability {
	var capabilities caps.Capability // capabilities for chifra transactions
	capabilities = capabilities.Add(caps.Default)
	capabilities = capabilities.Add(caps.Caching)
	capabilities = capabilities.Add(caps.Ether)
	capabilities = capabilities.Add(caps.Names)
	// EXISTING_CODE
	// EXISTING_CODE
	return capabilities
}

func ResetOptions(testMode bool) {
	// We want to keep writer between command file calls
	w := GetOptions().Globals.Writer
	opts := TransactionsOptions{}
	globals.SetDefaults(&opts.Globals)
	opts.Globals.TestMode = testMode
	opts.Globals.Writer = w
	opts.Globals.Caps = getCaps()
	// EXISTING_CODE
	// EXISTING_CODE
	defaultTransactionsOptions = opts
}

func (opts *TransactionsOptions) getCaches() (caches map[walk.CacheType]bool) {
	// EXISTING_CODE
	caches = map[walk.CacheType]bool{
		walk.Cache_Transactions: true,
		walk.Cache_Traces:       opts.CacheTraces || (opts.Globals.Cache && (opts.Traces || opts.Uniq)),
		walk.Cache_Abis:         opts.Articulate,
	}
	// EXISTING_CODE
	return
}

// EXISTING_CODE
// EXISTING_CODE
