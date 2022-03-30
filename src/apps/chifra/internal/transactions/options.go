// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package transactionsPkg

import (
	"fmt"
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient/ens"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

type TransactionsOptions struct {
	Transactions []string
	Articulate   bool
	Trace        bool
	Uniq         bool
	Reconcile    string
	Cache        bool
	Globals      globals.GlobalOptions
	BadFlag      error
}

var transactionsCmdLineOptions TransactionsOptions

func (opts *TransactionsOptions) TestLog() {
	logger.TestLog(len(opts.Transactions) > 0, "Transactions: ", opts.Transactions)
	logger.TestLog(opts.Articulate, "Articulate: ", opts.Articulate)
	logger.TestLog(opts.Trace, "Trace: ", opts.Trace)
	logger.TestLog(opts.Uniq, "Uniq: ", opts.Uniq)
	logger.TestLog(len(opts.Reconcile) > 0, "Reconcile: ", opts.Reconcile)
	logger.TestLog(opts.Cache, "Cache: ", opts.Cache)
	opts.Globals.TestLog()
}

func (opts *TransactionsOptions) ToCmdLine() string {
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
	options += fmt.Sprintf("%s", "") // silence go compiler for auto gen
	return options
}

func FromRequest(w http.ResponseWriter, r *http.Request) *TransactionsOptions {
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
	opts.Globals = *globals.FromRequest(w, r)
	// EXISTING_CODE
	opts.Reconcile = ens.ConvertOneEns(opts.Globals.Chain, opts.Reconcile)
	// EXISTING_CODE

	return opts
}

func TransactionsFinishParse(args []string) *TransactionsOptions {
	opts := GetOptions()
	// EXISTING_CODE
	opts.Transactions = args
	opts.Reconcile = ens.ConvertOneEns(opts.Globals.Chain, opts.Reconcile)
	// EXISTING_CODE
	return opts
}

func GetOptions() *TransactionsOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &transactionsCmdLineOptions
}
