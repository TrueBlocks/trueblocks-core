// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package scrapePkg

// EXISTING_CODE
import (
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"

	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunScrape handles the scrape command for the command line. Returns error only as per cobra.
func RunScrape(cmd *cobra.Command, args []string) error {
	opts := scrapeFinishParse(args)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.SetWriterForCommand("scrape", &opts.Globals)
	return opts.ScrapeInternal()
}

// ServeScrape handles the scrape command for the API. Returns an error.
func ServeScrape(w http.ResponseWriter, r *http.Request) error {
	opts := scrapeFinishParseApi(w, r)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("scrape", w, &opts.Globals)
	err := opts.ScrapeInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("scrape", err, &opts.Globals)
	return err
}

// ScrapeInternal handles the internal workings of the scrape command. Returns an error.
func (opts *ScrapeOptions) ScrapeInternal() error {
	var err error
	if err = opts.validateScrape(); err != nil {
		return err
	}

	timer := logger.NewTimer()
	msg := "chifra scrape"
	// EXISTING_CODE
	// EXISTING_CODE
	if opts.Touch > 0 {
		err = opts.HandleTouch()
	} else {
		err = opts.HandleShow()
	}
	timer.Report(msg)

	return err
}

// GetScrapeOptions returns the options for this tool so other tools may use it.
func GetScrapeOptions(args []string, g *globals.GlobalOptions) *ScrapeOptions {
	ret := scrapeFinishParse(args)
	if g != nil {
		ret.Globals = *g
	}
	return ret
}
