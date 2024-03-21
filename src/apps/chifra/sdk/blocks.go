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

	blocks "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/blocks"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
)

// Blocks provides an interface to the command line chifra blocks through the SDK.
func Blocks(w io.Writer, values url.Values) error {
	blocks.ResetOptions(sdkTestMode)
	opts := blocks.BlocksFinishParseInternal(w, values)
	outputHelpers.EnableCommand("blocks", true)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("blocks", w, &opts.Globals)
	err := opts.BlocksInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("blocks", err, &opts.Globals)

	return err
}

// EXISTING_CODE
// EXISTING_CODE
