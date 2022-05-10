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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"

	"github.com/spf13/cobra"
)

// EXISTING_CODE

func RunScrape(cmd *cobra.Command, args []string) error {
	opts := ScrapeFinishParse(args)

	err := opts.ValidateScrape()
	if err != nil {
		return err
	}

	// EXISTING_CODE
	if opts.Blaze {
		opts.ScrapeBlocks()

	} else if opts.Reset != utils.NOPOS {
		return opts.Globals.PassItOn("blockScrape", opts.Globals.Chain, opts.ToCmdLine(), opts.Globals.ToCmdLine())

	} else {
		var wg sync.WaitGroup

		if hasIndexerFlag(args[0]) {
			wg.Add(1)
			IndexScraper = NewScraper(colors.Yellow, "IndexScraper", opts.Sleep, opts.Globals.LogLevel)
			go opts.RunIndexScraper(&wg)
		}

		if hasMonitorsFlag(args[0]) {
			wg.Add(1)
			MonitorScraper = NewScraper(colors.Magenta, "MonitorScraper", opts.Sleep, opts.Globals.LogLevel)
			go opts.RunMonitorScraper(&wg)
		}

		wg.Wait()
	}
	return nil
	// EXISTING_CODE
}

func ServeScrape(w http.ResponseWriter, r *http.Request) bool {
	opts := ScrapeFinishParseApi(w, r)

	err := opts.ValidateScrape()
	if err != nil {
		output.RespondWithError(w, http.StatusInternalServerError, err)
		return true
	}

	// EXISTING_CODE
	// TODO: Can we disable certain things from running in server mode?
	// return opts.Globals.PassItOn("blockScrape", opts.Globals.Chain, opts.ToCmdLine(), opts.Globals.ToCmdLine())
	return false
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
