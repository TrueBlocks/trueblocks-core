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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	sdk "github.com/TrueBlocks/trueblocks-sdk/v4"
)

// EXISTING_CODE

// DoExport tests the Export sdk function
func DoExport() {
	file.EstablishFolder("sdkFuzzer-output/export")
	opts := sdk.ExportOptions{}
	ShowHeader("DoExport", opts)

	globs := globals
	topics := fuzzTopics
	fourbytes := fuzzFourbytes
	accounting := []bool{false, true}
	articulate := []bool{false, true}
	cacheTraces := []bool{false, true}
	relevant := []bool{false, true}
	emitter := fuzzEmitters
	topic := fuzzTopics
	reverted := []bool{false, true}
	asset := fuzzAssets
	// Option 'flow.enum' is an emum
	factory := []bool{false, true}
	unripe := []bool{false, true}
	reversed := []bool{false, true}
	noZero := []bool{false, true}
	// firstBlock is a <blknum> --other
	// lastBlock is a <blknum> --other
	// firstRecord is not fuzzed
	// maxRecords is not fuzzed
	// Fuzz Loop
	// EXISTING_CODE
	flow := []sdk.ExportFlow{sdk.NoEF, sdk.EFIn, sdk.EFOut, sdk.EFZero}
	_ = topic
	_ = fourbytes
	_ = articulate
	baseFn := "export/export"
	opts = sdk.ExportOptions{
		Addrs:       fuzzAddresses,
		FirstRecord: 0,
		MaxRecords:  10,
	}

	for _, g := range globs {
		opts.Globals = g
		fn := getFilename(baseFn, &opts.Globals)
		TestExportExport(reverted, unripe, accounting, reversed, asset, flow, fn, &opts)
		TestExportApps(noZero, fn, &opts)
		TestExport("receipts", "", fn, &opts)
		TestExportLogs(emitter, topics, relevant, fn, &opts)
		TestExportTraces(cacheTraces, factory, fn, &opts)
		// TestExport("neighbors", "", fn, &opts)
		TestExport("balances", "", fn, &opts)
		TestExport("withdrawals", "", fn, &opts)
		TestExport("statements", "", fn, &opts)
	}

	/*
	   -a, --articulate          articulate transactions, traces, logs, and outputs
	   -F, --first_block uint    first block to process (inclusive)
	   -L, --last_block uint     last block to process (inclusive)
	   -c, --first_record uint   the first record to process
	   -e, --max_records uint    the maximum number of records to process (default 250)
	*/

	// Load string `json:"load,omitempty"`
	// EXISTING_CODE
	Wait()
}

func TestExport(which, value, fn string, opts *sdk.ExportOptions) {
	fn = strings.Replace(fn, ".json", "-"+which+".json", 1)
	// EXISTING_CODE
	fn = strings.ReplaceAll(fn, "-true", "t")
	fn = strings.ReplaceAll(fn, "-false", "f")
	fn = strings.ReplaceAll(fn, ".json", "") + ".json"
	// EXISTING_CODE

	switch which {
	case "export":
		if export, _, err := opts.Export(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Transaction](fn, export); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "appearances":
		if appearances, _, err := opts.ExportAppearances(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Appearance](fn, appearances); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "receipts":
		if receipts, _, err := opts.ExportReceipts(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Receipt](fn, receipts); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "logs":
		if logs, _, err := opts.ExportLogs(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Log](fn, logs); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "traces":
		if traces, _, err := opts.ExportTraces(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Trace](fn, traces); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "neighbors":
		if neighbors, _, err := opts.ExportNeighbors(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Message](fn, neighbors); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "statements":
		if statements, _, err := opts.ExportStatements(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Statement](fn, statements); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "balances":
		if balances, _, err := opts.ExportBalances(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.State](fn, balances); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "withdrawals":
		if withdrawals, _, err := opts.ExportWithdrawals(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Withdrawal](fn, withdrawals); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "count":
		if count, _, err := opts.ExportCount(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Monitor](fn, count); err != nil {
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
func TestExportApps(noZero []bool, fn string, opt *sdk.ExportOptions) {
	opts := *opt
	for _, n := range noZero {
		opts.NoZero = n
		if n {
			fn += "-noZero"
		}
		TestExport("appearances", "", fn, &opts)
		TestExport("count", "", fn, &opts)
	}
}

func TestExportLogs(emitter, topics []string, relevant []bool, fn string, opt *sdk.ExportOptions) {
	for _, e := range emitter {
		for _, t := range topics {
			for _, r := range relevant {
				opts := *opt
				if len(e) > 0 {
					opts.Emitter = []string{e}
				}
				if len(t) > 0 {
					opts.Topic = []string{t}
				}
				opts.Relevant = r
				if r {
					fn += "-relevant"
				}
				ff := fn + "-" + e + "-" + t
				TestExport("logs", "", ff, &opts)
			}
		}
	}
}

func TestExportTraces(cacheTraces, factor []bool, fn string, opt *sdk.ExportOptions) {
	for _, c := range cacheTraces {
		for _, f := range factor {
			opts := *opt
			opts.CacheTraces = c
			opts.Factory = f
			if c {
				fn += "-cacheTraces"
			}
			if f {
				fn += "-factory"
			}
			TestExport("traces", "", fn, &opts)
		}
	}
}

func TestExportExport(reverted, unripe, accounting, reversed []bool, asset []string, flow []sdk.ExportFlow, fn string, opts *sdk.ExportOptions) {
	for _, r := range reverted {
		for _, u := range unripe {
			for _, a := range accounting {
				for _, rv := range reversed {
					opts := *opts
					opts.Reverted = r
					opts.Unripe = u
					opts.Accounting = a
					opts.Reversed = rv
					ff := fmt.Sprintf("%s--%t-%t-%t-%t", fn, r, u, a, rv)
					TestExport("export", "", ff, &opts)
					for _, as := range asset {
						opts.Asset = []string{as}
						ff += fmt.Sprintf("-%s", as)
						TestExport("statements", "", ff, &opts)
					}
				}
			}
		}
	}
}

// EXISTING_CODE
