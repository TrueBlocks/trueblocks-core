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

	abis "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/abis"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
)

// Abis provides an interface to the command line chifra abis through the SDK.
func Abis(rCtx output.RenderCtx, w io.Writer, values url.Values) error {
	abis.ResetOptions(sdkTestMode)
	opts := abis.AbisFinishParseInternal(w, values)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("abis", w, &opts.Globals)
	err := opts.AbisInternal(rCtx)
	outputHelpers.CloseJsonWriterIfNeededApi("abis", err, &opts.Globals)

	return err
}

// EXISTING_CODE
// EXISTING_CODE
