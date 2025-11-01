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

	explore "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/internal/explore"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/output"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/output/helpers"
)

// Explore provides an interface to the command line chifra explore through the SDK.
func Explore(rCtx *output.RenderCtx, w io.Writer, values url.Values) error {
	explore.ResetOptions(sdkTestMode)
	opts := explore.ExploreFinishParseInternal(w, values)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("explore", w, &opts.Globals)
	err := opts.ExploreInternal(rCtx)
	outputHelpers.CloseJsonWriterIfNeededApi("explore", err, &opts.Globals)

	return err
}

// EXISTING_CODE
// EXISTING_CODE
