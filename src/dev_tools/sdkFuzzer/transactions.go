// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */
package main

// EXISTING_CODE
import (
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/logger"
	sdk "github.com/TrueBlocks/trueblocks-sdk/v6"
)

// EXISTING_CODE

// DoTransactions tests the Transactions sdk function
func DoTransactions() {
	file.EstablishFolder("sdkFuzzer-output/transactions")
	opts := sdk.TransactionsOptions{}
	ShowHeader("DoTransactions", opts)

	globs := globals
	articulate := []bool{false, true}
	// Option 'flow.enum' is an emum
	emitter := fuzzEmitters
	topic := fuzzTopics
	cacheTraces := []bool{false, true}
	// Fuzz Loop
	// EXISTING_CODE
	_ = cacheTraces
	_ = emitter
	_ = topic
	cts := []bool{false, true}
	for _, art := range articulate {
		for _, c := range cts {
			opts = sdk.TransactionsOptions{
				TransactionIds: fuzzTransactions,
			}
			baseFn := "transactions/transactions"
			opts.Articulate = art
			opts.CacheTraces = c
			if art {
				baseFn += "-articulate"
			}
			if c {
				baseFn += "-cacheTraces"
			}
			for _, g := range globs {
				opts.Globals = g
				fn := getFilename(baseFn, &opts.Globals)
				TestTransactions("transactions", "", fn, &opts)
				fn = getFilename(baseFn+"-logs", &opts.Globals)
				TestTransactions("logs", "", fn, &opts)
				fn = getFilename(baseFn+"-traces", &opts.Globals)
				TestTransactions("traces", "", fn, &opts)
			}
		}
	}

	// for _, t := range topic {
	// 	for _, e := range emitter {
	// 		opts := sdk.TransactionsOptions{
	// 			TransactionIds: fuzzTransactions,
	// 		}
	// 		baseFn := "transactions/transactions-logs"
	// 		opts.Emitter = []string{e}
	// 		if len(e) > 0 {
	// 			baseFn += "-emitter"
	// 		}
	// 		opts.Topic = []string{t}
	// 		if len(t) > 0 {
	// 			baseFn += "-topic"
	// 		}
	// 		for _, g := range globs {
	// 			opts.Globals = g
	// 			fn := getFilename(baseFn, &opts.Globals)
	// 			TestTransactions("logs", "", fn, &opts)
	// 		}
	// 	}
	// }

	// flows := []sdk.TransactionsFlow{sdk.NoTF, sdk.TFFrom, sdk.TFTo}
	// for _, f := range flows {
	// 	opts := sdk.TransactionsOptions{
	// 		TransactionIds: fuzzTransactions,
	// 	}
	// 	baseFn := "transactions/transactions-uniq"
	// 	opts.Flow = f
	// 	baseFn += "-flow-" + f.String()
	// 	for _, g := range globs {
	// 		opts.Globals = g
	// 		fn := getFilename(baseFn, &opts.Globals)
	// 		TestTransactions("uniq", "", fn, &opts)
	// 	}
	// }

	// Not tested
	// func (opts *TransactionsOptions) TransactionsSeed() ([]bool, *types.MetaData, error) {
	// EXISTING_CODE
	Wait()
}

func TestTransactions(which, value, fn string, opts *sdk.TransactionsOptions) {
	fn = strings.Replace(fn, ".json", "-"+which+".json", 1)
	// EXISTING_CODE
	// EXISTING_CODE

	switch which {
	case "transactions":
		if transactions, _, err := opts.Transactions(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile(fn, transactions); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "traces":
		if traces, _, err := opts.TransactionsTraces(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile(fn, traces); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "uniq":
		if uniq, _, err := opts.TransactionsUniq(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile(fn, uniq); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "logs":
		if logs, _, err := opts.TransactionsLogs(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile(fn, logs); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	default:
		ReportError(fn, opts, fmt.Errorf("unknown which: %s", which))
		logger.Fatal("Quitting...")
		return
	}
}

// EXISTING_CODE
// EXISTING_CODE
