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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// scrapeCmd represents the scrape command
var scrapeCmd = &cobra.Command{
	Use:     usageScrape,
	Short:   shortScrape,
	Long:    longScrape,
	Version: versionText,
	PreRun: outputHelpers.PreRunWithJsonWriter("scrape", func() *globals.GlobalOptions {
		return &scrapePkg.GetOptions().Globals
	}),
	RunE: file.RunWithFileSupport("scrape", scrapePkg.RunScrape, scrapePkg.ResetOptions),
	PostRun: outputHelpers.PostRunWithJsonWriter(func() *globals.GlobalOptions {
		return &scrapePkg.GetOptions().Globals
	}),
}

const usageScrape = `scrape [flags]`

const shortScrape = "scan the chain and update the TrueBlocks index of appearances"

const longScrape = `Purpose:
  Scan the chain and update the TrueBlocks index of appearances.`

const notesScrape = ``

func init() {
	var capabilities = caps.Default // Additional global caps for chifra scrape
	// EXISTING_CODE
	capabilities = capabilities.Remove(caps.Fmt)
	capabilities = capabilities.Remove(caps.NoHeader)
	capabilities = capabilities.Remove(caps.File)
	capabilities = capabilities.Remove(caps.Output)
	capabilities = capabilities.Remove(caps.Append)
	// EXISTING_CODE

	scrapeCmd.Flags().SortFlags = false

	scrapeCmd.Flags().Uint64VarP(&scrapePkg.GetOptions().BlockCnt, "block_cnt", "n", 2000, "maximum number of blocks to process per pass")
	scrapeCmd.Flags().BoolVarP(&scrapePkg.GetOptions().Pin, "pin", "i", false, "pin new chunks (requires locally-running IPFS daemon or --remote)")
	scrapeCmd.Flags().BoolVarP(&scrapePkg.GetOptions().Remote, "remote", "r", false, "pin new chunks to the gateway (requires pinning service keys)")
	scrapeCmd.Flags().Float64VarP(&scrapePkg.GetOptions().Sleep, "sleep", "s", 14, "seconds to sleep between scraper passes")
	scrapeCmd.Flags().Uint64VarP(&scrapePkg.GetOptions().StartBlock, "start_block", "l", 0, "first block to visit when scraping (snapped back to most recent snap_to_grid mark)")
	scrapeCmd.Flags().Uint64VarP(&scrapePkg.GetOptions().RunCount, "run_count", "u", 0, "run the scraper this many times, then quit (hidden)")
	scrapeCmd.Flags().Uint64VarP(&scrapePkg.GetOptions().Settings.Apps_per_chunk, "apps_per_chunk", "", 200000, "the number of appearances to build into a chunk before consolidating it (hidden)")
	scrapeCmd.Flags().Uint64VarP(&scrapePkg.GetOptions().Settings.Snap_to_grid, "snap_to_grid", "", 100000, "an override to apps_per_chunk to snap-to-grid at every modulo of this value, this allows easier corrections to the index (hidden)")
	scrapeCmd.Flags().Uint64VarP(&scrapePkg.GetOptions().Settings.First_snap, "first_snap", "", 0, "the first block at which snap_to_grid is enabled (hidden)")
	scrapeCmd.Flags().Uint64VarP(&scrapePkg.GetOptions().Settings.Unripe_dist, "unripe_dist", "", 28, "the distance (in blocks) from the front of the chain under which (inclusive) a block is considered unripe (hidden)")
	scrapeCmd.Flags().Uint64VarP(&scrapePkg.GetOptions().Settings.Channel_count, "channel_count", "", 20, "number of concurrent processing channels (hidden)")
	scrapeCmd.Flags().BoolVarP(&scrapePkg.GetOptions().Settings.Allow_missing, "allow_missing", "", false, "do not report errors for blockchains that contain blocks with zero addresses (hidden)")
	if os.Getenv("TEST_MODE") != "true" {
		scrapeCmd.Flags().MarkHidden("run_count")
		scrapeCmd.Flags().MarkHidden("apps_per_chunk")
		scrapeCmd.Flags().MarkHidden("snap_to_grid")
		scrapeCmd.Flags().MarkHidden("first_snap")
		scrapeCmd.Flags().MarkHidden("unripe_dist")
		scrapeCmd.Flags().MarkHidden("channel_count")
		scrapeCmd.Flags().MarkHidden("allow_missing")
	}
	globals.InitGlobals(scrapeCmd, &scrapePkg.GetOptions().Globals, capabilities)

	scrapeCmd.SetUsageTemplate(UsageWithNotes(notesScrape))
	scrapeCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand(scrapeCmd)
}
