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

	state "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/state"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
)

// State provides an interface to the command line chifra state through the SDK.
func State(rCtx *output.RenderCtx, w io.Writer, values url.Values) error {
	state.ResetOptions(sdkTestMode)
	opts := state.StateFinishParseInternal(w, values)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("state", w, &opts.Globals)
	err := opts.StateInternal(rCtx)
	outputHelpers.CloseJsonWriterIfNeededApi("state", err, &opts.Globals)

	return err
}

// EXISTING_CODE
// EXISTING_CODE
