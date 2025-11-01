// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package cmd

// EXISTING_CODE
import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/internal/globals"
	scrapePkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/internal/scrape"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/file"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// scrapeCmd represents the scrape command
var scrapeCmd = &cobra.Command{
	Use:     usageScrape,
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

const longScrape = `Purpose:
  Scan the chain and update the TrueBlocks index of appearances.`

const notesScrape = `
Notes:
  - The --touch option may only be used for blocks after the latest scraped block (if any). It will be snapped back to the latest snap_to block.
  - This command requires your RPC to provide trace data. See the README for more information.
  - The --notify option requires proper configuration. Additionally, IPFS must be running locally. See the README.md file.`

func init() {
	var capabilities caps.Capability // capabilities for chifra scrape
	capabilities = capabilities.Add(caps.Verbose)
	capabilities = capabilities.Add(caps.Version)
	capabilities = capabilities.Add(caps.Noop)
	capabilities = capabilities.Add(caps.NoColor)
	capabilities = capabilities.Add(caps.Chain)

	scrapeCmd.Flags().SortFlags = false

	scrapeCmd.Flags().Uint64VarP(&scrapePkg.GetOptions().BlockCnt, "block_cnt", "n", 2000, `maximum number of blocks to process per pass`)
	scrapeCmd.Flags().Float64VarP(&scrapePkg.GetOptions().Sleep, "sleep", "s", 14, `seconds to sleep between scraper passes`)
	scrapeCmd.Flags().StringVarP(&scrapePkg.GetOptions().Publisher, "publisher", "P", "", `for some query options, the publisher of the index (hidden)`)
	scrapeCmd.Flags().Uint64VarP((*uint64)(&scrapePkg.GetOptions().Touch), "touch", "l", 0, `first block to visit when scraping (snapped back to most recent snap_to_grid mark)`)
	scrapeCmd.Flags().Uint64VarP(&scrapePkg.GetOptions().RunCount, "run_count", "u", 0, `run the scraper this many times, then quit`)
	scrapeCmd.Flags().BoolVarP(&scrapePkg.GetOptions().DryRun, "dry_run", "d", false, `show the configuration that would be applied if run,no changes are made`)
	scrapeCmd.Flags().BoolVarP(&scrapePkg.GetOptions().Notify, "notify", "o", false, `enable the notify feature`)
	scrapeCmd.Flags().Uint64VarP(&scrapePkg.GetOptions().Settings.AppsPerChunk, "apps_per_chunk", "", 2000000, `the number of appearances to build into a chunk before consolidating it (hidden)`)
	scrapeCmd.Flags().Uint64VarP(&scrapePkg.GetOptions().Settings.SnapToGrid, "snap_to_grid", "", 250000, `an override to apps_per_chunk to snap-to-grid at every modulo of this value, this allows easier corrections to the index (hidden)`)
	scrapeCmd.Flags().Uint64VarP(&scrapePkg.GetOptions().Settings.FirstSnap, "first_snap", "", 2000000, `the first block at which snap_to_grid is enabled (hidden)`)
	scrapeCmd.Flags().Uint64VarP(&scrapePkg.GetOptions().Settings.UnripeDist, "unripe_dist", "", 28, `the distance (in blocks) from the front of the chain under which (inclusive) a block is considered unripe (hidden)`)
	scrapeCmd.Flags().Uint64VarP(&scrapePkg.GetOptions().Settings.ChannelCount, "channel_count", "", 20, `number of concurrent processing channels (hidden)`)
	scrapeCmd.Flags().BoolVarP(&scrapePkg.GetOptions().Settings.AllowMissing, "allow_missing", "", false, `do not report errors for blockchains that contain blocks with zero addresses (hidden)`)
	if !base.IsTestMode() {
		_ = scrapeCmd.Flags().MarkHidden("publisher")
		_ = scrapeCmd.Flags().MarkHidden("apps_per_chunk")
		_ = scrapeCmd.Flags().MarkHidden("snap_to_grid")
		_ = scrapeCmd.Flags().MarkHidden("first_snap")
		_ = scrapeCmd.Flags().MarkHidden("unripe_dist")
		_ = scrapeCmd.Flags().MarkHidden("channel_count")
		_ = scrapeCmd.Flags().MarkHidden("allow_missing")
	}
	globals.InitGlobals("scrape", scrapeCmd, &scrapePkg.GetOptions().Globals, capabilities)

	scrapeCmd.SetUsageTemplate(UsageWithNotes(notesScrape))
	scrapeCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand(scrapeCmd)
}
