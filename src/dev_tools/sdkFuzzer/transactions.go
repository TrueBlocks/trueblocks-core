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

	// Flow           TransactionsFlow `json:"flow,omitempty"`
	// Emitter        []string         `json:"emitter,omitempty"`
	// Topic          []string         `json:"topic,omitempty"`
	// func (opts *TransactionsOptions) TransactionsUniq() ([]types.Appearance, *types.MetaData, error) {
	// func (opts *TransactionsOptions) TransactionsSeed() ([]bool, *types.MetaData, error) {
	// NoTF   TransactionsFlow = 0
	// TFFrom                  = 1 << iota
	// TFTo

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
	}
}
