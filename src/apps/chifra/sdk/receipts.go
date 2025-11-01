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

	receipts "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/internal/receipts"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/output"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/output/helpers"
)

// Receipts provides an interface to the command line chifra receipts through the SDK.
func Receipts(rCtx *output.RenderCtx, w io.Writer, values url.Values) error {
	receipts.ResetOptions(sdkTestMode)
	opts := receipts.ReceiptsFinishParseInternal(w, values)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("receipts", w, &opts.Globals)
	err := opts.ReceiptsInternal(rCtx)
	outputHelpers.CloseJsonWriterIfNeededApi("receipts", err, &opts.Globals)

	return err
}

// EXISTING_CODE
// EXISTING_CODE
