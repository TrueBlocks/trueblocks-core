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
	"strings"

	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// EXISTING_CODE

// DoExport tests the Export sdk function
func DoExport() {
	file.EstablishFolder("sdkFuzzer-output/export")
	opts := sdk.ExportOptions{}
	ShowHeader("DoExport", opts)

	globs := noRaw(globals)
	baseFn := "export/export"

	// EXISTING_CODE
	opts = sdk.ExportOptions{
		Addrs:       testAddrs,
		FirstRecord: 0,
		MaxRecords:  10,
		// Articulate: true,
	}
	// Addrs       []string    `json:"addrs,omitempty"`
	// Topics      []string    `json:"topics,omitempty"`
	// Fourbytes   []string    `json:"fourbytes,omitempty"`
	// Accounting  bool        `json:"accounting,omitempty"`
	// Articulate  bool        `json:"articulate,omitempty"`
	// CacheTraces bool        `json:"cacheTraces,omitempty"`
	// FirstRecord uint64      `json:"firstRecord,omitempty"`
	// MaxRecords  uint64      `json:"maxRecords,omitempty"`
	// Relevant    bool        `json:"relevant,omitempty"`
	// Emitter     []string    `json:"emitter,omitempty"`
	// Topic       []string    `json:"topic,omitempty"`
	// Reverted    bool        `json:"reverted,omitempty"`
	// Asset       []string    `json:"asset,omitempty"`
	// Flow        ExportFlow  `json:"flow,omitempty"`
	// Factory     bool        `json:"factory,omitempty"`
	// Unripe      bool        `json:"unripe,omitempty"`
	// Load        string      `json:"load,omitempty"`
	// Reversed    bool        `json:"reversed,omitempty"`
	// NoZero      bool        `json:"noZero,omitempty"`
	// FirstBlock  base.Blknum `json:"firstBlock,omitempty"`
	// LastBlock   base.Blknum `json:"lastBlock,omitempty"`
	// NoEF ExportFlow = 0
	// EFIn            = 1 << iota
	// EFOut
	// EFZero
	// export,command,default|caching|ether|
	for _, g := range globs {
		opts.Globals = g
		fn := baseFn
		TestExport("export", "", fn, &opts)
		TestExport("appearances", "", fn, &opts)
		TestExport("receipts", "", fn, &opts)
		TestExport("logs", "", fn, &opts)
		TestExport("traces", "", fn, &opts)
		TestExport("neighbors", "", fn, &opts)
		TestExport("statements", "", fn, &opts)
		TestExport("balances", "", fn, &opts)
		TestExport("withdrawals", "", fn, &opts)
		TestExport("count", "", fn, &opts)
	}
	// EXISTING_CODE
	Wait()
}

func TestExport(which, value, fn string, opts *sdk.ExportOptions) {
	fn = strings.Replace(fn, ".json", "-"+which+".json", 1)
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
	// 	if neighbors, _, err := opts.ExportNeigbors(); err != nil {
	// 		ReportError(fn, opts, err)
	// 	} else {
	// 		if err := SaveToFile[types.Neighbor](fn, neighbors); err != nil {
	// 			ReportError2(fn, err)
	// 		} else {
	// 			ReportOkay(fn)
	// 		}
	// 	}
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
		if withdrawls, _, err := opts.ExportWithdrawals(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Withdrawal](fn, withdrawls); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "count":
		if counts, _, err := opts.ExportCount(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.AppearanceCount](fn, counts); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	}
}

// EXISTING_CODE
// EXISTING_CODE
