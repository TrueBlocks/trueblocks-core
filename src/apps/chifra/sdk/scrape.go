// Copyright 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package sdk

import (
	"io"
	"net/url"
	"os"

	scrape "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/scrape"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
)

// Scrape does chifra scrape
func Scrape(w io.Writer, options map[string]string) error {
	values := make(url.Values)
	for key, val := range options {
		values.Set(key, val)
	}

	scrape.ResetOptions(false)
	opts := scrape.ScrapeFinishParseInternal(os.Stdout, values)
	outputHelpers.EnableCommand("scrape", true)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("scrape", w, &opts.Globals)
	err := opts.ScrapeInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("scrape", err, &opts.Globals)

	return err
}

