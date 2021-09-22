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

import (
	"fmt"

	"github.com/spf13/cobra"
)

// scrapeCmd represents the scrape command
var scrapeCmd = &cobra.Command{
	Use:   "scrape",
	Short: "A brief description of your command",
	Long: `A longer description that spans multiple lines and likely contains examples
and usage of using your command. For example:

Cobra is a CLI library for Go that empowers applications.
This application is a tool to generate the needed files
to quickly create a Cobra application.`,
	Run: func(cmd *cobra.Command, args []string) {
		fmt.Println("scrape called")
	},
}

func init() {
	rootCmd.AddCommand(scrapeCmd)
	scrapeCmd.SetHelpTemplate(getHelpTextScrape())
}

func getHelpTextScrape() string {
	return `chifra argc: 5 [1:scrape] [2:--help] [3:--verbose] [4:2] 
chifra scrape --help --verbose 2 
chifra scrape argc: 5 [1:--help] [2:--verbose] [3:2] [4:--scrape] 
chifra scrape --help --verbose 2 --scrape 
PROG_NAME = [chifra scrape]

  Usage:    chifra scrape [-p|-s|-v|-h]  
  Purpose:  Scan the chain and update the TrueBlocks index of appearances.

  Where:
    -p  (--pin)           pin new chunks (and blooms) to IPFS (requires Pinata key and running IPFS node)
    -s  (--sleep <double>)the number of seconds to sleep between passes (default 14)
    -x  (--fmt <val>)     export format, one of [none|json*|txt|csv|api]
    -v  (--verbose)       set verbose level (optional level defaults to 1)
    -h  (--help)          display this help screen

  Configurable Items:
    - n_blocks: maximum number of blocks to process (defaults to 5000).
    - n_block_procs: number of concurrent block channels for blaze.
    - n_addr_procs: number of concurrent address channels for blaze.

  Powered by TrueBlocks
`
}
