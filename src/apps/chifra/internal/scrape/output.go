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
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"

	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunScrape handles the scrape command for the command line. Returns error only as per cobra.
func RunScrape(cmd *cobra.Command, args []string) (err error) {
	opts := scrapeFinishParse(args)
	outputHelpers.SetEnabledForCmds("scrape", opts.IsPorted())
	outputHelpers.SetWriterForCommand("scrape", &opts.Globals)
	// EXISTING_CODE
	// EXISTING_CODE
	err, _ = opts.ScrapeInternal()
	return
}

// ServeScrape handles the scrape command for the API. Returns error and a bool if handled
func ServeScrape(w http.ResponseWriter, r *http.Request) (err error, handled bool) {
	opts := scrapeFinishParseApi(w, r)
	outputHelpers.SetEnabledForCmds("scrape", opts.IsPorted())
	outputHelpers.InitJsonWriterApi("scrape", w, &opts.Globals)
	// EXISTING_CODE
	// EXISTING_CODE
	err, handled = opts.ScrapeInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("scrape", err, &opts.Globals)
	return
}

// ScrapeInternal handles the internal workings of the scrape command.  Returns error and a bool if handled
func (opts *ScrapeOptions) ScrapeInternal() (err error, handled bool) {
	err = opts.validateScrape()
	if err != nil {
		return err, true
	}

	timer := logger.NewTimer()
	msg := "chifra scrape"
	// EXISTING_CODE
	handled = true
	err = opts.HandleScrape() // Note this never returns
	// EXISTING_CODE
	timer.Report(msg)

	return
}

// GetScrapeOptions returns the options for this tool so other tools may use it.
func GetScrapeOptions(args []string, g *globals.GlobalOptions) *ScrapeOptions {
	ret := scrapeFinishParse(args)
	if g != nil {
		ret.Globals = *g
	}
	return ret
}

func (opts *ScrapeOptions) IsPorted() (ported bool) {
	// EXISTING_CODE
	ported = true
	// EXISTING_CODE
	return
}

// EXISTING_CODE
func getConfigCmdsFromArgs() map[string]string {
	configs := make(map[string]string, 10)
	for i := 0; i < len(os.Args); i++ {
		arg := os.Args[i]
		next := ""
		if i < len(os.Args)-1 {
			next = os.Args[i+1]
		}
		switch arg {
		case "--apps_per_chunk":
			configs["appsPerChunk"] = next
		case "--snap_to_grid":
			configs["snapToGrid"] = next
		case "--first_snap":
			configs["firstSnap"] = next
		case "--unripe_dist":
			configs["unripeDist"] = next
		case "--channel_count":
			configs["channelCount"] = next
		case "--allow_missing":
			configs["allowMissing"] = "true"
		}
	}
	return configs
}

// EXISTING_CODE
