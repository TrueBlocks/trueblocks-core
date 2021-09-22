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

// abisCmd represents the abis command
var abisCmd = &cobra.Command{
	Use:   "abis",
	Short: "A brief description of your command",
	Long: `A longer description that spans multiple lines and likely contains examples
and usage of using your command. For example:

Cobra is a CLI library for Go that empowers applications.
This application is a tool to generate the needed files
to quickly create a Cobra application.`,
	Run: func(cmd *cobra.Command, args []string) {
		fmt.Println("abis called")
	},
}

func init() {
	rootCmd.AddCommand(abisCmd)

	abisCmd.SetHelpTemplate(getHelpTextAbis())
}

func getHelpTextAbis() string {
	return `chifra argc: 5 [1:abis] [2:--help] [3:--verbose] [4:2] 
chifra abis --help --verbose 2 
chifra abis argc: 4 [1:--help] [2:--verbose] [3:2] 
chifra abis --help --verbose 2 
PROG_NAME = [chifra abis]

  Usage:    chifra abis [-k|-s|-f|-v|-h] <address> [address...]  
  Purpose:  Fetches the ABI for a smart contract.

  Where:
    addrs                 list of one or more smart contracts whose ABI to grab from EtherScan (required)
    -k  (--known)         load common 'known' ABIs from cache
    -s  (--sol <str>)     file name of .sol file from which to create a new known abi (without .sol)
    -f  (--find <str>)    try to search for a function declaration given a four byte code

    #### Hidden options
    -o  (--source)        show the source of the ABI information
    -c  (--classes)       generate classDefinitions folder and class definitions
    #### Hidden options

    -x  (--fmt <val>)     export format, one of [none|json*|txt|csv|api]
    -v  (--verbose)       set verbose level (optional level defaults to 1)
    -h  (--help)          display this help screen

  Notes:
    - Solidity files found in the local folder with the name '<address>.sol' are converted to an ABI prior to processing (and then removed).

  Powered by TrueBlocks
`
}
