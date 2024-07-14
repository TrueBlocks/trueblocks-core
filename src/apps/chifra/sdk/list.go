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

	list "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/list"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
)

// List provides an interface to the command line chifra list through the SDK.
func List(rCtx output.RenderCtx, w io.Writer, values url.Values) error {
	list.ResetOptions(sdkTestMode)
	opts := list.ListFinishParseInternal(w, values)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("list", w, &opts.Globals)
	err := opts.ListInternal(rCtx)
	outputHelpers.CloseJsonWriterIfNeededApi("list", err, &opts.Globals)

	return err
}

// EXISTING_CODE
// EXISTING_CODE
