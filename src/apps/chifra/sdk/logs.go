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

	logs "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/logs"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
)

// Logs provides an interface to the command line chifra logs through the SDK.
func Logs(rCtx output.RenderCtx, w io.Writer, values url.Values) error {
	logs.ResetOptions(sdkTestMode)
	opts := logs.LogsFinishParseInternal(w, values)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("logs", w, &opts.Globals)
	err := opts.LogsInternal(rCtx)
	outputHelpers.CloseJsonWriterIfNeededApi("logs", err, &opts.Globals)

	return err
}

// EXISTING_CODE
// EXISTING_CODE
