// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package cmd

// EXISTING_CODE
import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	scrapePkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/scrape"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// scrapeCmd represents the scrape command
var scrapeCmd = &cobra.Command{
	Use:     usageScrape,
	Short:   shortScrape,
	Long:    longScrape,
	Version: "GHC-TrueBlocks//0.17.0-alpha",
	RunE:    scrapePkg.RunScrape,
}

var usageScrape = `scrape [flags] [mode...]

Arguments:
  modes - which scraper(s) to control (required)
	One or more of [ indexer | monitors | both ]`

var shortScrape = "scan the chain and update (and optionally pin) the TrueBlocks index of appearances"

var longScrape = `Purpose:
  Scan the chain and update (and optionally pin) the TrueBlocks index of appearances.`

var notesScrape = `
Notes:
  - if no mode is presented, chifra scrape indexer --action run is assumed.
  - the --pin and --publish options require an API to the pinning service.
  - the --n_* related options allow you to tune the scrapers.`

func init() {
	scrapeCmd.Flags().SortFlags = false

	scrapeCmd.Flags().StringVarP(&scrapePkg.Options.Action, "action", "a", "", `command to apply to the specified scrape
One of [ toggle | run | restart | pause | quit ]`)
	scrapeCmd.Flags().Float64VarP(&scrapePkg.Options.Sleep, "sleep", "s", 14, "seconds to sleep between scraper passes")
	scrapeCmd.Flags().BoolVarP(&scrapePkg.Options.Pin, "pin", "p", false, "pin chunks (and blooms) to IPFS as they are created (requires pinning service)")
	scrapeCmd.Flags().BoolVarP(&scrapePkg.Options.Publish, "publish", "u", false, "after pinning the chunk, publish it to UnchainedIndex")
	scrapeCmd.Flags().Uint64VarP(&scrapePkg.Options.BlockCnt, "block_cnt", "n", 2000, "maximum number of blocks to process per pass")
	scrapeCmd.Flags().Uint64VarP(&scrapePkg.Options.BlockChanCnt, "block_chan_cnt", "b", 10, "number of concurrent block processing channels (hidden)")
	scrapeCmd.Flags().Uint64VarP(&scrapePkg.Options.AddrChanCnt, "addr_chan_cnt", "d", 20, "number of concurrent address processing channels (hidden)")
	if os.Getenv("TEST_MODE") != "true" {
		scrapeCmd.Flags().MarkHidden("block_chan_cnt")
		scrapeCmd.Flags().MarkHidden("addr_chan_cnt")
	}
	globals.InitGlobals(scrapeCmd, &scrapePkg.Options.Globals)

	scrapeCmd.SetUsageTemplate(UsageWithNotes(notesScrape))
	scrapeCmd.SetOut(os.Stderr)

	chifraCmd.AddCommand(scrapeCmd)
}
