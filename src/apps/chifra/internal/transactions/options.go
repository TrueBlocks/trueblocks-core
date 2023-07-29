// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package transactionsPkg

import (
	"encoding/json"
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
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
	AccountFor     string                   `json:"accountFor,omitempty"`     // Reconcile the transaction as per the provided address
	CacheTraces    bool                     `json:"cacheTraces,omitempty"`    // Force the transaction's traces into the cache
	Decache        bool                     `json:"decache,omitempty"`        // Removes a transactions and any traces in the transaction from the cache
	Source         bool                     `json:"source,omitempty"`         // Find the source of the funds sent to the receiver
	Globals        globals.GlobalOptions    `json:"globals,omitempty"`        // The global options
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
	logger.TestLog(len(opts.AccountFor) > 0, "AccountFor: ", opts.AccountFor)
	logger.TestLog(opts.CacheTraces, "CacheTraces: ", opts.CacheTraces)
	logger.TestLog(opts.Decache, "Decache: ", opts.Decache)
	logger.TestLog(opts.Source, "Source: ", opts.Source)
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *TransactionsOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "  ")
	return string(b)
}

// transactionsFinishParseApi finishes the parsing for server invocations. Returns a new TransactionsOptions.
func transactionsFinishParseApi(w http.ResponseWriter, r *http.Request) *TransactionsOptions {
	copy := defaultTransactionsOptions
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
		case "accountFor":
			opts.AccountFor = value[0]
		case "cacheTraces":
			opts.CacheTraces = true
		case "decache":
			opts.Decache = true
		case "source":
			opts.Source = true
		default:
			if !copy.Globals.Caps.HasKey(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "transactions")
				return opts
			}
		}
	}
	opts.Globals = *globals.GlobalsFinishParseApi(w, r)
	// EXISTING_CODE
	opts.AccountFor, _ = rpcClient.ConvertOneEns(opts.Globals.Chain, opts.AccountFor)
	// EXISTING_CODE

	return opts
}

// transactionsFinishParse finishes the parsing for command line invocations. Returns a new TransactionsOptions.
func transactionsFinishParse(args []string) *TransactionsOptions {
	opts := GetOptions()
	opts.Globals.FinishParse(args)
	defFmt := "txt"
	// EXISTING_CODE
	opts.Transactions = args
	opts.AccountFor, _ = rpcClient.ConvertOneEns(opts.Globals.Chain, opts.AccountFor)
	// EXISTING_CODE
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

func ResetOptions() {
	// We want to keep writer between command file calls
	w := GetOptions().Globals.Writer
	defaultTransactionsOptions = TransactionsOptions{}
	globals.SetDefaults(&defaultTransactionsOptions.Globals)
	defaultTransactionsOptions.Globals.Writer = w
	capabilities := caps.Default // Additional global caps for chifra transactions
	// EXISTING_CODE
	capabilities = capabilities.Add(caps.Caching)
	capabilities = capabilities.Add(caps.Raw)
	capabilities = capabilities.Add(caps.Ether)
	capabilities = capabilities.Add(caps.Wei)
	// EXISTING_CODE
	defaultTransactionsOptions.Globals.Caps = capabilities
}

// EXISTING_CODE
//

// CacheState returns booleans indicating if transaction cache and trace
// cache should be writable (usually it is set by the user using --cache_txs
// and --cache_traces flags)
func (opts *TransactionsOptions) CacheState() (bool, bool, bool) {
	return opts.Globals.Cache, true, opts.CacheTraces
}

// EXISTING_CODE
