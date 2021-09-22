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

// tokensCmd represents the tokens command
var tokensCmd = &cobra.Command{
	Use:   "tokens",
	Short: "A brief description of your command",
	Long: `A longer description that spans multiple lines and likely contains examples
and usage of using your command. For example:

Cobra is a CLI library for Go that empowers applications.
This application is a tool to generate the needed files
to quickly create a Cobra application.`,
	Run: func(cmd *cobra.Command, args []string) {
		fmt.Println("tokens called")
	},
}

func init() {
	rootCmd.AddCommand(tokensCmd)
	tokensCmd.SetHelpTemplate(getHelpTextTokens())
}

func getHelpTextTokens() string {
	return `chifra argc: 5 [1:tokens] [2:--help] [3:--verbose] [4:2] 
chifra tokens --help --verbose 2 
chifra tokens argc: 4 [1:--help] [2:--verbose] [3:2] 
chifra tokens --help --verbose 2 
PROG_NAME = [chifra tokens]

  Usage:    chifra tokens [-p|-b|-n|-v|-h] <address> <address> [address...] [block...]  
  Purpose:  Retrieve token balance(s) for one or more addresses at given block(s).

  Where:
    addrs                 two or more addresses (0x...), the first is an ERC20 token, balances for the rest are reported (required)
    blocks                an optional list of one or more blocks at which to report balances, defaults to 'latest'
    -p  (--parts <val>)   one or more parts of the token information to retreive, one or more of [name|symbol|decimals|totalSupply|version|none|all*]
    -b  (--by_acct)       consider each address an ERC20 token except the last, whose balance is reported for each token
    -n  (--no_zero)       suppress the display of zero balance accounts
    -x  (--fmt <val>)     export format, one of [none|json*|txt|csv|api]
    -v  (--verbose)       set verbose level (optional level defaults to 1)
    -h  (--help)          display this help screen

  Notes:
    - An address must start with '0x' and be forty-two characters long.
    - blocks may be a space-separated list of values, a start-end range, a special, or any combination.
    - If the token contract(s) from which you request balances are not ERC20 compliant, the results are undefined.
    - If the queried node does not store historical state, the results are undefined.
    - special blocks are detailed under chifra when --list.

  Powered by TrueBlocks
`
}
