// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package receiptsPkg

import (
	"encoding/json"
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

// ReceiptsOptions provides all command options for the chifra receipts command.
type ReceiptsOptions struct {
	Transactions   []string                 `json:"transactions,omitempty"`   // A space-separated list of one or more transaction identifiers
	TransactionIds []identifiers.Identifier `json:"transactionIds,omitempty"` // Transaction identifiers
	Articulate     bool                     `json:"articulate,omitempty"`     // Articulate the retrieved data if ABIs can be found
	Globals        globals.GlobalOptions    `json:"globals,omitempty"`        // The global options
	BadFlag        error                    `json:"badFlag,omitempty"`        // An error flag if needed
	// EXISTING_CODE
	// EXISTING_CODE
}

var defaultReceiptsOptions = ReceiptsOptions{}

// testLog is used only during testing to export the options for this test case.
func (opts *ReceiptsOptions) testLog() {
	logger.TestLog(len(opts.Transactions) > 0, "Transactions: ", opts.Transactions)
	logger.TestLog(opts.Articulate, "Articulate: ", opts.Articulate)
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *ReceiptsOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "  ")
	return string(b)
}

// receiptsFinishParseApi finishes the parsing for server invocations. Returns a new ReceiptsOptions.
func receiptsFinishParseApi(w http.ResponseWriter, r *http.Request) *ReceiptsOptions {
	copy := defaultReceiptsOptions
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
		default:
			if !copy.Globals.Caps.HasKey(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "receipts")
				return opts
			}
		}
	}
	opts.Globals = *globals.GlobalsFinishParseApi(w, r)
	// EXISTING_CODE
	// EXISTING_CODE

	return opts
}

// receiptsFinishParse finishes the parsing for command line invocations. Returns a new ReceiptsOptions.
func receiptsFinishParse(args []string) *ReceiptsOptions {
	opts := GetOptions()
	opts.Globals.FinishParse(args)
	defFmt := "txt"
	// EXISTING_CODE
	opts.Transactions = args
	// EXISTING_CODE
	if len(opts.Globals.Format) == 0 || opts.Globals.Format == "none" {
		opts.Globals.Format = defFmt
	}
	return opts
}

func GetOptions() *ReceiptsOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &defaultReceiptsOptions
}

func ResetOptions() {
	// We want to keep writer between command file calls
	w := GetOptions().Globals.Writer
	defaultReceiptsOptions = ReceiptsOptions{}
	globals.SetDefaults(&defaultReceiptsOptions.Globals)
	defaultReceiptsOptions.Globals.Writer = w
	capabilities := caps.Default // Additional global caps for use with --file option
	// EXISTING_CODE
	capabilities = capabilities.Add(caps.Caching)
	capabilities = capabilities.Add(caps.Raw)
	// EXISTING_CODE
	defaultReceiptsOptions.Globals.Caps = capabilities
}

// EXISTING_CODE
// EXISTING_CODE
