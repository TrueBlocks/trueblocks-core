// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package transactionsPkg

import (
	"encoding/json"
	"fmt"
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient/ens"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

// TransactionsOptions provides all command options for the chifra transactions command.
type TransactionsOptions struct {
	Transactions   []string                 `json:"transactions,omitempty"`   // A space-separated list of one or more transaction identifiers
	TransactionIds []identifiers.Identifier `json:"transactionIds,omitempty"` // Transaction identifiers
	Articulate     bool                     `json:"articulate,omitempty"`     // Articulate the retrieved data if ABIs can be found
	Trace          bool                     `json:"trace,omitempty"`          // Include the transaction's traces in the results
	Uniq           bool                     `json:"uniq,omitempty"`           // Display a list of uniq addresses found in the transaction
	Flow           string                   `json:"flow,omitempty"`           // For the uniq option only, export only from or to (including trace from or to)
	Reconcile      string                   `json:"reconcile,omitempty"`      // Please use statements option instead
	AccountFor     string                   `json:"accountFor,omitempty"`     // Reconcile the transaction as per the provided address
	Cache          bool                     `json:"cache,omitempty"`          // Force the results of the query into the tx cache (and the trace cache if applicable)
	Source         bool                     `json:"source,omitempty"`         // Find the source of the funds sent to the receiver
	Globals        globals.GlobalOptions    `json:"globals,omitempty"`        // The global options
	BadFlag        error                    `json:"badFlag,omitempty"`        // An error flag if needed
}

var defaultTransactionsOptions = TransactionsOptions{}

// testLog is used only during testing to export the options for this test case.
func (opts *TransactionsOptions) testLog() {
	logger.TestLog(len(opts.Transactions) > 0, "Transactions: ", opts.Transactions)
	logger.TestLog(opts.Articulate, "Articulate: ", opts.Articulate)
	logger.TestLog(opts.Trace, "Trace: ", opts.Trace)
	logger.TestLog(opts.Uniq, "Uniq: ", opts.Uniq)
	logger.TestLog(len(opts.Flow) > 0, "Flow: ", opts.Flow)
	logger.TestLog(len(opts.AccountFor) > 0, "AccountFor: ", opts.AccountFor)
	logger.TestLog(opts.Cache, "Cache: ", opts.Cache)
	logger.TestLog(opts.Source, "Source: ", opts.Source)
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *TransactionsOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "  ")
	return string(b)
}

// getEnvStr allows for custom environment strings when calling to the system (helps debugging).
func (opts *TransactionsOptions) getEnvStr() []string {
	envStr := []string{}
	// EXISTING_CODE
	// EXISTING_CODE
	return envStr
}

// toCmdLine converts the option to a command line for calling out to the system.
func (opts *TransactionsOptions) toCmdLine() string {
	options := ""
	if opts.Articulate {
		options += " --articulate"
	}
	if opts.Trace {
		options += " --trace"
	}
	if opts.Uniq {
		options += " --uniq"
	}
	if len(opts.Flow) > 0 {
		options += " --flow " + opts.Flow
	}
	if len(opts.AccountFor) > 0 {
		options += " --account_for " + opts.AccountFor
	}
	if opts.Cache {
		options += " --cache"
	}
	if opts.Source {
		options += " --source"
	}
	options += " " + strings.Join(opts.Transactions, " ")
	// EXISTING_CODE
	// EXISTING_CODE
	options += fmt.Sprintf("%s", "") // silence compiler warning for auto gen
	return options
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
		case "trace":
			opts.Trace = true
		case "uniq":
			opts.Uniq = true
		case "flow":
			opts.Flow = value[0]
		case "reconcile":
			opts.Reconcile = value[0]
		case "accountFor":
			opts.AccountFor = value[0]
		case "cache":
			opts.Cache = true
		case "source":
			opts.Source = true
		default:
			if !globals.IsGlobalOption(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "transactions")
				return opts
			}
		}
	}
	opts.Globals = *globals.GlobalsFinishParseApi(w, r)
	// EXISTING_CODE
	opts.AccountFor = ens.ConvertOneEns(opts.Globals.Chain, opts.AccountFor)
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
	opts.AccountFor = ens.ConvertOneEns(opts.Globals.Chain, opts.AccountFor)
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
}
