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

	status "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/status"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
)

// Status provides an interface to the command line chifra status through the SDK.
func Status(w io.Writer, values url.Values) error {
	status.ResetOptions(sdkTestMode)
	opts := status.StatusFinishParseInternal(w, values)
	outputHelpers.EnableCommand("status", true)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("status", w, &opts.Globals)
	err := opts.StatusInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("status", err, &opts.Globals)

	return err
}

// EXISTING_CODE
// EXISTING_CODE
