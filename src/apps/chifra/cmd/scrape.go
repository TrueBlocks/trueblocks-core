/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
/*
 * Parts of this file were generated with makeClass --gocmds. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */
package cmd

import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/spf13/cobra"
)

// scrapeCmd represents the scrape command
var scrapeCmd = &cobra.Command{
	Use:   usageScrape,
	Short: shortScrape,
	Long:  longScrape,
	Run:   runScrape,
	Args:  validateScrapeArgs,
}

var usageScrape = `scrape [flags] [mode...]

Arguments:
  modes - which scraper(s) to control (indexer is default)
	One or more of [ indexer | monitors | both ]`

var shortScrape = "scan the chain and update (and optionally pin) the TrueBlocks index of appearances"

var longScrape = `Purpose:
  Scan the chain and update (and optionally pin) the TrueBlocks index of appearances.`

var notesScrape = `
Notes:
  - if no mode is presented, chifra scrape indexer --action run is assumed.
  - the --pin and --publish options require an API to the pinning service.
  - the --n_* related options allow you to tune the scrapers.`

type scrapeOptionsType struct {
	Action         string
	Sleep          float64
	Pin            bool
	Publish        bool
	Block_Cnt      uint64
	Block_Chan_Cnt uint64
	Addr_Chan_Cnt  uint64
}

var ScrapeOpts scrapeOptionsType

func init() {
	scrapeCmd.SetOut(os.Stderr)

	scrapeCmd.Flags().SortFlags = false
	scrapeCmd.PersistentFlags().SortFlags = false
	scrapeCmd.Flags().StringVarP(&ScrapeOpts.Action, "action", "a", "", `command to apply to the specified scrape
One of [ toggle | run | restart | pause | quit ]`)
	scrapeCmd.Flags().Float64VarP(&ScrapeOpts.Sleep, "sleep", "s", 14, "seconds to sleep between scraper passes")
	scrapeCmd.Flags().BoolVarP(&ScrapeOpts.Pin, "pin", "p", false, "pin chunks (and blooms) to IPFS as they are created (requires pinning service)")
	scrapeCmd.Flags().BoolVarP(&ScrapeOpts.Publish, "publish", "u", false, "after pinning the chunk, publish it to UnchainedIndex")
	scrapeCmd.Flags().Uint64VarP(&ScrapeOpts.Block_Cnt, "block_cnt", "n", 2000, "maximum number of blocks to process per pass")
	scrapeCmd.Flags().Uint64VarP(&ScrapeOpts.Block_Chan_Cnt, "block_chan_cnt", "b", 10, "number of concurrent block processing channels (hidden)")
	scrapeCmd.Flags().Uint64VarP(&ScrapeOpts.Addr_Chan_Cnt, "addr_chan_cnt", "d", 20, "number of concurrent address processing channels (hidden)")
	if !utils.IsTestMode() {
		scrapeCmd.Flags().MarkHidden("block_chan_cnt")
		scrapeCmd.Flags().MarkHidden("addr_chan_cnt")
	}
	scrapeCmd.Flags().SortFlags = false
	scrapeCmd.PersistentFlags().SortFlags = false

	scrapeCmd.SetUsageTemplate(UsageWithNotes(notesScrape))
	rootCmd.AddCommand(scrapeCmd)
}

func TestLogScrape(args []string) {
	if !utils.IsTestMode() {
		return
	}
}

// EXISTING_CODE
// EXISTING_CODE
