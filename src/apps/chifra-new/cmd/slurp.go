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

// slurpCmd represents the slurp command
var slurpCmd = &cobra.Command{
	Use:   "slurp",
	Short: "A brief description of your command",
	Long: `A longer description that spans multiple lines and likely contains examples
and usage of using your command. For example:

Cobra is a CLI library for Go that empowers applications.
This application is a tool to generate the needed files
to quickly create a Cobra application.`,
	Run: func(cmd *cobra.Command, args []string) {
		fmt.Println("slurp called")
	},
}

func init() {
	rootCmd.AddCommand(slurpCmd)
	slurpCmd.SetHelpTemplate(getHelpTextSlurp())
}

func getHelpTextSlurp() string {
	return `chifra argc: 5 [1:slurp] [2:--help] [3:--verbose] [4:2] 
chifra slurp --help --verbose 2 
chifra slurp argc: 4 [1:--help] [2:--verbose] [3:2] 
chifra slurp --help --verbose 2 
PROG_NAME = [chifra slurp]

  Usage:    chifra slurp [-t|-p|-v|-h] <address> [address...] [block...]  
  Purpose:  Fetch data from EtherScan for any address.

  Where:
    addrs                 one or more addresses to slurp from Etherscan (required)
    blocks                an optional range of blocks to slurp
    -t  (--types <val>)   one or more types of transactions to request, one or more of [ext*|int|token|nfts|miner|uncles|all]
    -p  (--appearances)   show only the blocknumer.tx_id appearances of the exported transactions
    -x  (--fmt <val>)     export format, one of [none|json*|txt|csv|api]
    -v  (--verbose)       set verbose level (optional level defaults to 1)
    -h  (--help)          display this help screen

  Notes:
    - Portions of this software are Powered by Etherscan.io APIs.

  Powered by TrueBlocks
`
}
