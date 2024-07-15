// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package sdk

import (
	"io"
	"net/url"

	transactions "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/transactions"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
)

// Transactions provides an interface to the command line chifra transactions through the SDK.
func Transactions(rCtx *output.RenderCtx, w io.Writer, values url.Values) error {
	transactions.ResetOptions(sdkTestMode)
	opts := transactions.TransactionsFinishParseInternal(w, values)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("transactions", w, &opts.Globals)
	err := opts.TransactionsInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("transactions", err, &opts.Globals)

	return err
}

// EXISTING_CODE
// EXISTING_CODE
