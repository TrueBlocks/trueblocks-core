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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/utils"
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

var usageScrape = `scrape [flags]`

var shortScrape = "scan the chain and update the TrueBlocks index of appearances"

var longScrape = `Purpose:
  Scan the chain and update the TrueBlocks index of appearances.`

var notesScrape = ``

type scrapeOptionsType struct {
	pin           bool
	sleep         float64
	n_blocks      uint64
	n_block_procs uint64
	n_addr_procs  uint64
}

var ScrapeOpts scrapeOptionsType

func init() {
	scrapeCmd.SetOut(os.Stderr)

	scrapeCmd.Flags().SortFlags = false
	scrapeCmd.PersistentFlags().SortFlags = false
	scrapeCmd.Flags().BoolVarP(&ScrapeOpts.pin, "pin", "p", false, "pin new chunks (and blooms) to IPFS (requires Pinata key and running IPFS node)")
	scrapeCmd.Flags().Float64VarP(&ScrapeOpts.sleep, "sleep", "s", 14, "the number of seconds to sleep between passes")
	scrapeCmd.Flags().Uint64VarP(&ScrapeOpts.n_blocks, "n_blocks", "n", 2000, "maximum number of blocks to process")
	scrapeCmd.Flags().Uint64VarP(&ScrapeOpts.n_block_procs, "n_block_procs", "b", 10, "number of concurrent block channels for blaze (hidden)")
	scrapeCmd.Flags().Uint64VarP(&ScrapeOpts.n_addr_procs, "n_addr_procs", "a", 20, "number of concurrent address channels for blaze (hidden)")
	if !utils.IsTestMode() {
		scrapeCmd.Flags().MarkHidden("n_block_procs")
		scrapeCmd.Flags().MarkHidden("n_addr_procs")
	}
	scrapeCmd.Flags().SortFlags = false
	scrapeCmd.PersistentFlags().SortFlags = false

	scrapeCmd.SetUsageTemplate(UsageWithNotes(notesScrape))
	rootCmd.AddCommand(scrapeCmd)
}

// EXISTING_CODE
// EXISTING_CODE
