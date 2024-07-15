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

	export "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/export"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
)

// Export provides an interface to the command line chifra export through the SDK.
func Export(rCtx *output.RenderCtx, w io.Writer, values url.Values) error {
	export.ResetOptions(sdkTestMode)
	opts := export.ExportFinishParseInternal(w, values)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("export", w, &opts.Globals)
	err := opts.ExportInternal(rCtx)
	outputHelpers.CloseJsonWriterIfNeededApi("export", err, &opts.Globals)

	return err
}

// EXISTING_CODE
// EXISTING_CODE
