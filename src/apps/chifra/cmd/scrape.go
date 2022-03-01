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

var usageScrape = `scrape [flags] [mode...]

Arguments:
  modes - which scraper(s) to control (required)
	One or more of [ indexer | monitors | both ]`

var shortScrape = "scan the chain and update (and optionally pin) the TrueBlocks index of appearances"

var longScrape = `Purpose:
  Scan the chain and update (and optionally pin) the TrueBlocks index of appearances.`

var notesScrape = ``

func init() {
	scrapeCmd.Flags().SortFlags = false

	scrapeCmd.Flags().StringVarP(&scrapePkg.GetOptions().Action, "action", "a", "", `command to apply to the specified scrape
One of [ toggle | run | restart | pause | quit ]`)
	scrapeCmd.Flags().Float64VarP(&scrapePkg.GetOptions().Sleep, "sleep", "s", 14, "seconds to sleep between scraper passes")
	scrapeCmd.Flags().BoolVarP(&scrapePkg.GetOptions().Pin, "pin", "p", false, "pin chunks (and blooms) to IPFS as they are created (requires pinning service)")
	scrapeCmd.Flags().BoolVarP(&scrapePkg.GetOptions().Publish, "publish", "u", false, "after pinning the chunk, publish it to UnchainedIndex")
	scrapeCmd.Flags().BoolVarP(&scrapePkg.GetOptions().Blaze, "blaze", "z", false, "invoke the blaze scraper to process blocks")
	scrapeCmd.Flags().Uint64VarP(&scrapePkg.GetOptions().BlockCnt, "block_cnt", "n", 2000, "maximum number of blocks to process per pass")
	scrapeCmd.Flags().Uint64VarP(&scrapePkg.GetOptions().BlockChanCnt, "block_chan_cnt", "b", 10, "number of concurrent block processing channels")
	scrapeCmd.Flags().Uint64VarP(&scrapePkg.GetOptions().AddrChanCnt, "addr_chan_cnt", "d", 20, "number of concurrent address processing channels")
	scrapeCmd.Flags().Uint64VarP(&scrapePkg.GetOptions().AppsPerChunk, "apps_per_chunk", "", 2000000, "the number of appearances to build into a chunk before consolidating it (hidden)")
	scrapeCmd.Flags().Uint64VarP(&scrapePkg.GetOptions().UnripeDist, "unripe_dist", "", 28, "the distance (in blocks) from the front of the chain under which (inclusive) a block is considered unripe (hidden)")
	scrapeCmd.Flags().Uint64VarP(&scrapePkg.GetOptions().SnapToGrid, "snap_to_grid", "", 100000, "an override to apps_per_chunk to snap-to-grid at every modulo of this value, this allows easier corrections to the index (hidden)")
	scrapeCmd.Flags().Uint64VarP(&scrapePkg.GetOptions().FirstSnap, "first_snap", "", 2250000, "the first block at which snap_to_grid is enabled (hidden)")
	scrapeCmd.Flags().BoolVarP(&scrapePkg.GetOptions().AllowMissing, "allow_missing", "", false, "do not report errors for blockchain that contain blocks with zero addresses (hidden)")
	scrapeCmd.Flags().Uint64VarP(&scrapePkg.GetOptions().NTestRuns, "n_test_runs", "", 0, "in live testing mode, the number of test runs to process before quitting (hidden)")
	scrapeCmd.Flags().Uint64VarP(&scrapePkg.GetOptions().StartBlock, "start_block", "l", 0, "first block to visit (available only for blaze scraper)")
	scrapeCmd.Flags().Uint64VarP(&scrapePkg.GetOptions().RipeBlock, "ripe_block", "r", 0, "blocks prior to this value are written to 'ripe' folder (available only for blaze scraper)")
	if os.Getenv("TEST_MODE") != "true" {
		scrapeCmd.Flags().MarkHidden("apps_per_chunk")
		scrapeCmd.Flags().MarkHidden("unripe_dist")
		scrapeCmd.Flags().MarkHidden("snap_to_grid")
		scrapeCmd.Flags().MarkHidden("first_snap")
		scrapeCmd.Flags().MarkHidden("allow_missing")
		scrapeCmd.Flags().MarkHidden("n_test_runs")
	}
	globals.InitGlobals(scrapeCmd, &scrapePkg.GetOptions().Globals)

	scrapeCmd.SetUsageTemplate(UsageWithNotes(notesScrape))
	scrapeCmd.SetOut(os.Stderr)

	chifraCmd.AddCommand(scrapeCmd)
}
