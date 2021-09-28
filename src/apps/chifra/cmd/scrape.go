package cmd

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

import (
	// EXISTING_CODE
	"fmt"
	"os"

	"github.com/spf13/cobra"
	// EXISTING_CODE
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

	// EXISTING_CODE
	// EXISTING_CODE
	scrapeCmd.Flags().SortFlags = false
	scrapeCmd.PersistentFlags().SortFlags = false
	scrapeCmd.Flags().BoolVarP(&ScrapeOpts.pin, "pin", "p", false, "pin new chunks (and blooms) to IPFS (requires Pinata key and running IPFS node)")
	scrapeCmd.Flags().Float64VarP(&ScrapeOpts.sleep, "sleep", "s", 0.0, "the number of seconds to sleep between passes (default 14)")
	scrapeCmd.Flags().Uint64VarP(&ScrapeOpts.n_blocks, "n_blocks", "n", 0, "maximum number of blocks to process (default 2000)")
	scrapeCmd.Flags().Uint64VarP(&ScrapeOpts.n_block_procs, "n_block_procs", "b", 0, "number of concurrent block channels for blaze (hidden)")
	scrapeCmd.Flags().Uint64VarP(&ScrapeOpts.n_addr_procs, "n_addr_procs", "a", 0, "number of concurrent address channels for blaze (hidden)")
	if IsTestMode() == false {
		scrapeCmd.Flags().MarkHidden("n_block_procs")
		scrapeCmd.Flags().MarkHidden("n_addr_procs")
	}
	scrapeCmd.Flags().SortFlags = false
	scrapeCmd.PersistentFlags().SortFlags = false
	// EXISTING_CODE
	// EXISTING_CODE

	scrapeCmd.SetUsageTemplate(HelpWithNotes(notesScrape))
	rootCmd.AddCommand(scrapeCmd)
}

func runScrape(cmd *cobra.Command, args []string) {
	options := ""
	if ScrapeOpts.pin {
		options += " --pin"
	}
	if ScrapeOpts.sleep > 0.0 {
		options += " --sleep " + fmt.Sprintf("%.1f", ScrapeOpts.sleep)
	}
	if ScrapeOpts.n_blocks > 0 {
		options += " --n_blocks " + fmt.Sprintf("%d", ScrapeOpts.n_blocks)
	}
	if ScrapeOpts.n_block_procs > 0 {
		options += " --n_block_procs " + fmt.Sprintf("%d", ScrapeOpts.n_block_procs)
	}
	if ScrapeOpts.n_addr_procs > 0 {
		options += " --n_addr_procs " + fmt.Sprintf("%d", ScrapeOpts.n_addr_procs)
	}
	arguments := ""
	for _, arg := range args {
		arguments += " " + arg
	}
	// EXISTING_CODE
	// EXISTING_CODE
	PassItOn(GetCommandPath("blockScrape"), options, arguments)
}

// EXISTING_CODE
// EXISTING_CODE

func validateScrapeArgs(cmd *cobra.Command, args []string) error {
	var err error
	// EXISTING_CODE
	// EXISTING_CODE
	err = validateGlobalFlags(cmd, args)
	if err != nil {
		return err
	}
	return nil
}
