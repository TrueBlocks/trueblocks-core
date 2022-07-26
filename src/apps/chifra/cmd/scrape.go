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
	Version: versionText,
	RunE:    scrapePkg.RunScrape,
}

const usageScrape = `scrape [flags]`

const shortScrape = "scan the chain and update (and optionally pin) the TrueBlocks index of appearances"

const longScrape = `Purpose:
  Scan the chain and update (and optionally pin) the TrueBlocks index of appearances.`

const notesScrape = ``

func init() {
	scrapeCmd.Flags().SortFlags = false

	scrapeCmd.Flags().BoolVarP(&scrapePkg.GetOptions().Pin, "pin", "p", false, "pin chunks (and blooms) to IPFS as they are created (requires ipfs)")
	scrapeCmd.Flags().Uint64VarP(&scrapePkg.GetOptions().BlockCnt, "block_cnt", "n", 2000, "maximum number of blocks to process per pass")
	scrapeCmd.Flags().Float64VarP(&scrapePkg.GetOptions().Sleep, "sleep", "s", 14, "seconds to sleep between scraper passes")
	if os.Getenv("TEST_MODE") != "true" {
		scrapeCmd.Flags().MarkHidden("modes")
	}
	globals.InitGlobals(scrapeCmd, &scrapePkg.GetOptions().Globals)

	scrapeCmd.SetUsageTemplate(UsageWithNotes(notesScrape))
	scrapeCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand(scrapeCmd)
}
