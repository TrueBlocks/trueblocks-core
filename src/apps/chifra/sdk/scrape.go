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

	scrape "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/internal/scrape"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/output"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/output/helpers"
)

// Scrape provides an interface to the command line chifra scrape through the SDK.
func Scrape(rCtx *output.RenderCtx, w io.Writer, values url.Values) error {
	scrape.ResetOptions(sdkTestMode)
	opts := scrape.ScrapeFinishParseInternal(w, values)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("scrape", w, &opts.Globals)
	err := opts.ScrapeInternal(rCtx)
	outputHelpers.CloseJsonWriterIfNeededApi("scrape", err, &opts.Globals)

	return err
}

// EXISTING_CODE
// EXISTING_CODE
