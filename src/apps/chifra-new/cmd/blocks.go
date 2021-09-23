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

// blocksCmd represents the blocks command
var blocksCmd = &cobra.Command{
	Use:   "blocks [flags] <block> [block...]",
	Short: "A brief description of your command",
	Long: `A longer description that spans multiple lines and likely contains examples
and usage of using your command. For example:

Cobra is a CLI library for Go that empowers applications.
This application is a tool to generate the needed files
to quickly create a Cobra application.`,
	Run: func(cmd *cobra.Command, args []string) {
		if RootOpts.oldHelp {
			fmt.Println(getHelpTextBlocks())
			return
		}
		fmt.Println("blocks called")
	},
}

func init() {
	rootCmd.AddCommand(blocksCmd)
}

func getHelpTextBlocks() string {
	return `chifra argc: 5 [1:blocks] [2:--help] [3:--verbose] [4:2] 
chifra blocks --help --verbose 2 
chifra blocks argc: 4 [1:--help] [2:--verbose] [3:2] 
chifra blocks --help --verbose 2 
PROG_NAME = [chifra blocks]

  Usage:    chifra blocks [-e|-U|-t|-a|-u|-n|-c|-o|-v|-h] <block> [block...]  
  Purpose:  Retrieve one or more blocks from the chain or local cache.

  Where:
    blocks                a space-separated list of one or more block identifiers (required)
    -e  (--hashes)        display only transaction hashes, default is to display full transaction detail
    -U  (--uncles)        display uncle blocks (if any) instead of the requested block
    -t  (--trace)         export the traces from the block as opposed to the block data
    -a  (--apps)          display only the list of address appearances in the block
    -u  (--uniq)          display only the list of uniq address appearances in the block
    -n  (--uniq_tx)       display only the list of uniq address appearances in each transaction
    -c  (--count)         display the number of the lists of appearances for --apps, --uniq, or --uniq_tx
    -o  (--cache)         force a write of the block to the cache

    #### Hidden options
    -l  (--list <num>)    summary list of blocks running backwards from latest block minus num
    -C  (--list_count <num>)the number of blocks to report for --list option
    #### Hidden options

    -x  (--fmt <val>)     export format, one of [none|json*|txt|csv|api]
    -v  (--verbose)       set verbose level (optional level defaults to 1)
    -h  (--help)          display this help screen

  Notes:
    - blocks is a space-separated list of values, a start-end range, a special, or any combination.
    - blocks may be specified as either numbers or hashes.
    - special blocks are detailed under chifra when --list.

  Powered by TrueBlocks
`
}
