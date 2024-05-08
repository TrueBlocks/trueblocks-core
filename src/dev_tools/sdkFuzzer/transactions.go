package main

import (
	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// DoTransactions tests the transactions sdk function
func DoTransactions() {
	file.EstablishFolder("sdkFuzzer-output/transactions")
	opts := sdk.TransactionsOptions{
		TransactionIds: testTransactions,
	}
	ShowHeader("DoTransactions", &opts)
	globs := globals

	art := []bool{false, true}
	cts := []bool{false, true}

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
				TestTransactions("txs", fn, &opts)
				fn = getFilename(baseFn+"-logs", &opts.Globals)
				TestTransactions("logs", fn, &opts)
				fn = getFilename(baseFn+"-traces", &opts.Globals)
				TestTransactions("traces", fn, &opts)
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
				TestTransactions("logs", fn, &opts)
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
			TestTransactions("uniq", fn, &opts)
		}
	}

	// Not tested
	// func (opts *TransactionsOptions) TransactionsSeed() ([]bool, *types.MetaData, error) {
}

func TestTransactions(which, fn string, opts *sdk.TransactionsOptions) {
	switch which {
	case "txs":
		if txs, _, err := opts.Transactions(); err != nil {
			ReportError(fn, err)
		} else {
			if err := SaveToFile[types.Transaction](fn, txs); err != nil {
				ReportError(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "logs":
		if logs, _, err := opts.TransactionsLogs(); err != nil {
			ReportError(fn, err)
		} else {
			if err := SaveToFile[types.Log](fn, logs); err != nil {
				ReportError(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "traces":
		if traces, _, err := opts.TransactionsTraces(); err != nil {
			ReportError(fn, err)
		} else {
			if err := SaveToFile[types.Trace](fn, traces); err != nil {
				ReportError(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "uniq":
		if apps, _, err := opts.TransactionsUniq(); err != nil {
			ReportError(fn, err)
		} else {
			if err := SaveToFile[types.Appearance](fn, apps); err != nil {
				ReportError(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	}
}
