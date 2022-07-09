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
	"fmt"
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"

	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunScrape handles the scrape command for the command line. Returns error only as per cobra.
func RunScrape(cmd *cobra.Command, args []string) (err error) {
	opts := scrapeFinishParse(args)
	// EXISTING_CODE
	// EXISTING_CODE
	err, _ = opts.ScrapeInternal()
	return
}

// ServeScrape handles the scrape command for the API. Returns error and a bool if handled
func ServeScrape(w http.ResponseWriter, r *http.Request) (err error, handled bool) {
	opts := scrapeFinishParseApi(w, r)
	// EXISTING_CODE
	// EXISTING_CODE
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

	} else {
		// Note this never returns
		err = opts.HandleScrape()

	}
	// EXISTING_CODE

	return
}

// EXISTING_CODE
func (opts *ScrapeOptions) GetSettingsStr(which string) string {
	switch which {
	case "TB_SETTINGS_BLOCKCNT":
		return which + "=" + fmt.Sprintf("%d", opts.BlockCnt)
	case "TB_SETTINGS_BLOCKCHANCNT":
		return which + "=" + fmt.Sprintf("%d", config.ReadBlockScrape(opts.Globals.Chain).Settings.Block_chan_cnt)
	case "TB_SETTINGS_ADDRCHANCNT":
		return which + "=" + fmt.Sprintf("%d", config.ReadBlockScrape(opts.Globals.Chain).Settings.Addr_chan_cnt)
	case "TB_SETTINGS_APPSPERCHUNK":
		return which + "=" + fmt.Sprintf("%d", config.ReadBlockScrape(opts.Globals.Chain).Settings.Apps_per_chunk)
	case "TB_SETTINGS_UNRIPEDIST":
		return which + "=" + fmt.Sprintf("%d", config.ReadBlockScrape(opts.Globals.Chain).Settings.Unripe_dist)
	case "TB_SETTINGS_SNAPTOGRID":
		return which + "=" + fmt.Sprintf("%d", config.ReadBlockScrape(opts.Globals.Chain).Settings.Snap_to_grid)
	case "TB_SETTINGS_FIRSTSNAP":
		return which + "=" + fmt.Sprintf("%d", config.ReadBlockScrape(opts.Globals.Chain).Settings.First_snap)
	case "TB_SETTINGS_ALLOWMISSING":
		return which + "=" + fmt.Sprintf("%t", config.ReadBlockScrape(opts.Globals.Chain).Settings.Allow_missing)
	}
	return ""
}

// EXISTING_CODE
