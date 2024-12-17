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

// DoReceipts tests the Receipts sdk function
func DoReceipts() {
	file.EstablishFolder("sdkFuzzer-output/receipts")
	opts := sdk.ReceiptsOptions{}
	ShowHeader("DoReceipts", opts)

	globs := noEther(globals)
	articulate := []bool{false, true}
	// Fuzz Loop
	// EXISTING_CODE
	// TransactionIds []string `json:"transactions,omitempty"`
	// Articulate     bool     `json:"articulate,omitempty"`
	// func (opts *ReceiptsOptions) Receipts() ([]types.Receipt, *types.MetaData, error) {

	// receipts,command,default|caching|
	opts = sdk.ReceiptsOptions{
		TransactionIds: append(firsts, []string{"17100101.1"}...),
	}
	for _, art := range articulate {
		baseFn := "receipts/receipts"
		if art {
			baseFn += "-articulate"
		}
		opts.Articulate = art
		for _, g := range globs {
			opts.Globals = g
			fn := getFilename(baseFn, &opts.Globals)
			TestReceipts("receipts", "", fn, &opts)
		}
	}
	// EXISTING_CODE
	Wait()
}

func TestReceipts(which, value, fn string, opts *sdk.ReceiptsOptions) {
	fn = strings.Replace(fn, ".json", "-"+which+".json", 1)
	// EXISTING_CODE
	// EXISTING_CODE

	switch which {
	case "receipts":
		if receipts, _, err := opts.Receipts(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Receipt](fn, receipts); err != nil {
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
