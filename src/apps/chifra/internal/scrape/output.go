// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package scrapePkg

// EXISTING_CODE
import (
	"net/http"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"

	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunScrape handles the scrape command for the command line. Returns error only as per cobra.
func RunScrape(cmd *cobra.Command, args []string) (err error) {
	opts := ScrapeFinishParse(args)
	// JINKY
	// JINKY
	err, _ = opts.ScrapeInternal()
	return
}

// ServeScrape handles the scrape command for the API. Returns error and a bool if handled
func ServeScrape(w http.ResponseWriter, r *http.Request) (err error, handled bool) {
	opts := ScrapeFinishParseApi(w, r)
	// JINKY
	// JINKY
	return opts.ScrapeInternal()
}

// ScrapeInternal handles the internal workings of the scrape command.  Returns error and a bool if handled
func (opts *ScrapeOptions) ScrapeInternal() (err error, handled bool) {
	err = opts.ValidateScrape()
	if err != nil {
		return err, true
	}

	// EXISTING_CODE
	if opts.Globals.ApiMode {
		return validate.Usage("chifra scrape is not available in API mode"), true
	}

	handled = true
	if opts.Blaze {
		err = opts.ScrapeBlocks()

	} else if opts.Reset != utils.NOPOS {
		err = opts.Globals.PassItOn("blockScrape", opts.Globals.Chain, opts.ToCmdLine(), opts.Globals.ToCmdLine())

	} else {
		var wg sync.WaitGroup

		if hasIndexerFlag(opts.Modes[0]) {
			wg.Add(1)
			IndexScraper = NewScraper(colors.Yellow, "IndexScraper", opts.Sleep, opts.Globals.LogLevel)
			// Note that this never returns
			go opts.RunIndexScraper(&wg)
		}

		if hasMonitorsFlag(opts.Modes[0]) {
			wg.Add(1)
			MonitorScraper = NewScraper(colors.Magenta, "MonitorScraper", opts.Sleep, opts.Globals.LogLevel)
			// Note that this never returns
			go opts.RunMonitorScraper(&wg)
		}

		wg.Wait()
	}
	// EXISTING_CODE

	return
}

// EXISTING_CODE
// EXISTING_CODE
