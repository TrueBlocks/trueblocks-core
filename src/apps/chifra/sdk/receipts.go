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

	receipts "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/receipts"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
)

// Receipts does chifra receipts
func Receipts(w io.Writer, options map[string]string) error {
	values := make(url.Values)
	for key, val := range options {
		values.Set(key, val)
	}

	receipts.ResetOptions(false)
	opts := receipts.ReceiptsFinishParseInternal(os.Stdout, values)
	outputHelpers.EnableCommand("receipts", true)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("receipts", w, &opts.Globals)
	err := opts.ReceiptsInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("receipts", err, &opts.Globals)

	return err
}

