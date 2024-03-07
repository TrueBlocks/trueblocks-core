// Copyright 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package sdk

import (
	"io"
	"net/url"
	"os"

	transactions "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/transactions"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
)

// Transactions provides an interface to the command line chifra transactions through the SDK.
func Transactions(w io.Writer, values url.Values) error {
	testMode := os.Getenv("TEST_MODE") == "true"
	transactions.ResetOptions(testMode)
	opts := transactions.TransactionsFinishParseInternal(w, values)
	outputHelpers.EnableCommand("transactions", true)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("transactions", w, &opts.Globals)
	err := opts.TransactionsInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("transactions", err, &opts.Globals)

	return err
}

// EXISTING_CODE
// EXISTING_CODE

