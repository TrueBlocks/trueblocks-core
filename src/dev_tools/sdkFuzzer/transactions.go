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

	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// EXISTING_CODE

// DoTransactions tests the Transactions sdk function
func DoTransactions() {
	file.EstablishFolder("sdkFuzzer-output/transactions")
	opts := sdk.TransactionsOptions{}
	ShowHeader("DoTransactions", opts)

	globs := globals
	// Option 'flow' is an enum
	// EXISTING_CODE
	art := []bool{false, true}
	cts := []bool{false, true}
	// opts = sdk.TransactionsOptions{
	// 	TransactionIds: testTransactions,
	// }
	// transactions,command,default|caching|ether|raw|
	for _, a := range art {
		for _, c := range cts {
			opts := sdk.TransactionsOptions{
				TransactionIds: testTransactions,
			}
			baseFn := "transactions/transactions"
			opts.Articulate = a
			opts.CacheTraces = c
			if a {
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

	emitters := []string{"", "0x5564886ca2c518d1964e5fcea4f423b41db9f561"}
	topics := []string{"", "0xa6697e974e6a320f454390be03f74955e8978f1a6971ea6730542e37b66179bc"}
	for _, t := range topics {
		for _, e := range emitters {
			opts := sdk.TransactionsOptions{
				TransactionIds: []string{"17100101.1"},
			}
			baseFn := "transactions/transactions-logs"
			if len(e) > 0 {
				opts.Emitter = []string{e}
				baseFn += "-emitter"
			}
			if len(t) > 0 {
				opts.Topic = []string{t}
				baseFn += "-topic"
			}
			for _, g := range globs {
				opts.Globals = g
				fn := getFilename(baseFn, &opts.Globals)
				TestTransactions("logs", "", fn, &opts)
			}
		}
	}

	flows := []sdk.TransactionsFlow{sdk.NoTF, sdk.TFFrom, sdk.TFTo}
	for _, f := range flows {
		opts := sdk.TransactionsOptions{
			TransactionIds: []string{"17100101.1"},
		}
		baseFn := "transactions/transactions-uniq"
		opts.Flow = f
		baseFn += "-flow-" + f.String()
		for _, g := range globs {
			opts.Globals = g
			fn := getFilename(baseFn, &opts.Globals)
			TestTransactions("uniq", "", fn, &opts)
		}
	}

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
			if err := SaveToFile[types.Transaction](fn, transactions); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "traces":
		if traces, _, err := opts.TransactionsTraces(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Trace](fn, traces); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "uniq":
		if uniq, _, err := opts.TransactionsUniq(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Appearance](fn, uniq); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "logs":
		if logs, _, err := opts.TransactionsLogs(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Log](fn, logs); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "seed":
		if seed, _, err := opts.TransactionsSeed(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[base.Address](fn, seed); err != nil {
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
