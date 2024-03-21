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

	slurp "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/slurp"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
)

// Slurp provides an interface to the command line chifra slurp through the SDK.
func Slurp(w io.Writer, values url.Values) error {
	slurp.ResetOptions(sdkTestMode)
	opts := slurp.SlurpFinishParseInternal(w, values)
	outputHelpers.EnableCommand("slurp", true)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("slurp", w, &opts.Globals)
	err := opts.SlurpInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("slurp", err, &opts.Globals)

	return err
}

// EXISTING_CODE
// EXISTING_CODE
