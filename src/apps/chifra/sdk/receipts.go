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

	receipts "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/receipts"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
)

// Receipts provides an interface to the command line chifra receipts through the SDK.
func Receipts(w io.Writer, values url.Values) error {
	receipts.ResetOptions(sdkTestMode)
	opts := receipts.ReceiptsFinishParseInternal(w, values)
	outputHelpers.EnableCommand("receipts", true)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("receipts", w, &opts.Globals)
	err := opts.ReceiptsInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("receipts", err, &opts.Globals)

	return err
}

// EXISTING_CODE
// EXISTING_CODE
