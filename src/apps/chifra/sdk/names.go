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

	names "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/internal/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/output"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/output/helpers"
)

// Names provides an interface to the command line chifra names through the SDK.
func Names(rCtx *output.RenderCtx, w io.Writer, values url.Values) error {
	names.ResetOptions(sdkTestMode)
	opts := names.NamesFinishParseInternal(w, values)
	// EXISTING_CODE
	var err1 error
	if err1 = opts.LoadCrudDataIfNeeded(nil); err1 != nil {
		return err1
	}
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("names", w, &opts.Globals)
	err := opts.NamesInternal(rCtx)
	outputHelpers.CloseJsonWriterIfNeededApi("names", err, &opts.Globals)

	return err
}

// EXISTING_CODE
// EXISTING_CODE
