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

	traces "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/internal/traces"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/output"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/output/helpers"
)

// Traces provides an interface to the command line chifra traces through the SDK.
func Traces(rCtx *output.RenderCtx, w io.Writer, values url.Values) error {
	traces.ResetOptions(sdkTestMode)
	opts := traces.TracesFinishParseInternal(w, values)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("traces", w, &opts.Globals)
	err := opts.TracesInternal(rCtx)
	outputHelpers.CloseJsonWriterIfNeededApi("traces", err, &opts.Globals)

	return err
}

// EXISTING_CODE
// EXISTING_CODE
