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

	chunks "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/chunks"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
)

// Chunks provides an interface to the command line chifra chunks through the SDK.
func Chunks(w io.Writer, values url.Values) error {
	chunks.ResetOptions(sdkTestMode)
	opts := chunks.ChunksFinishParseInternal(w, values)
	outputHelpers.EnableCommand("chunks", true)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("chunks", w, &opts.Globals)
	err := opts.ChunksInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("chunks", err, &opts.Globals)

	return err
}

// EXISTING_CODE
// EXISTING_CODE
