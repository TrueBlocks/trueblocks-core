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
	// a space-separated list of one or more transaction identifiers
	Transactions []string `json:"transactions,omitempty"`
	// transaction identifiers
	TransactionIds []identifiers.Identifier `json:"transactionIds,omitempty"`
	// articulate the retrieved data if ABIs can be found
	Articulate bool `json:"articulate,omitempty"`
	// include the transaction's traces in the results
	Trace bool `json:"trace,omitempty"`
	// display a list of uniq addresses found in the transaction
	Uniq bool `json:"uniq,omitempty"`
	// reconcile the transaction as per the provided address
	Reconcile string `json:"reconcile,omitempty"`
	// force the results of the query into the tx cache (and the trace cache if applicable)
	Cache bool `json:"cache,omitempty"`
	// the global options
	Globals globals.GlobalOptions `json:"globals,omitempty"`
	// an error flag if needed
	BadFlag error `json:"badFlag,omitempty"`
}

var transactionsCmdLineOptions TransactionsOptions

// testLog is used only during testing to export the options for this test case.
func (opts *TransactionsOptions) testLog() {
	logger.TestLog(len(opts.Transactions) > 0, "Transactions: ", opts.Transactions)
	logger.TestLog(opts.Articulate, "Articulate: ", opts.Articulate)
	logger.TestLog(opts.Trace, "Trace: ", opts.Trace)
	logger.TestLog(opts.Uniq, "Uniq: ", opts.Uniq)
	logger.TestLog(len(opts.Reconcile) > 0, "Reconcile: ", opts.Reconcile)
	logger.TestLog(opts.Cache, "Cache: ", opts.Cache)
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *TransactionsOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "\t")
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
	if len(opts.Reconcile) > 0 {
		options += " --reconcile " + opts.Reconcile
	}
	if opts.Cache {
		options += " --cache"
	}
	options += " " + strings.Join(opts.Transactions, " ")
	// EXISTING_CODE
	// EXISTING_CODE
	options += fmt.Sprintf("%s", "") // silence go compiler for auto gen
	return options
}

// transactionsFinishParseApi finishes the parsing for server invocations. Returns a new TransactionsOptions.
func transactionsFinishParseApi(w http.ResponseWriter, r *http.Request) *TransactionsOptions {
	opts := &TransactionsOptions{}
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
		case "reconcile":
			opts.Reconcile = value[0]
		case "cache":
			opts.Cache = true
		default:
			if !globals.IsGlobalOption(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "transactions")
				return opts
			}
		}
	}
	opts.Globals = *globals.GlobalsFinishParseApi(w, r)
	// EXISTING_CODE
	opts.Reconcile = ens.ConvertOneEns(opts.Globals.Chain, opts.Reconcile)
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
	opts.Reconcile = ens.ConvertOneEns(opts.Globals.Chain, opts.Reconcile)
	// EXISTING_CODE
	if len(opts.Globals.Format) == 0 || opts.Globals.Format == "none" {
		opts.Globals.Format = defFmt
	}
	return opts
}

func GetOptions() *TransactionsOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &transactionsCmdLineOptions
}
