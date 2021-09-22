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
	"os"

	"github.com/spf13/cobra"
)

// exploreCmd represents the explore command
var exploreCmd = &cobra.Command{
	Use:   "explore",
	Short: "Open an explorer for one or more addresses, blocks, or transactions",
	Long: `explore opens Etherscan (and other explorers -- including our own) to the block, 
transaction hash, or address you specify. It's a handy (configurable) way to open an explorer
from the command line, nothing more.`,
	Run: func(cmd *cobra.Command, args []string) {
		fmt.Println("explore called")
	},
}

func init() {
	rootCmd.AddCommand(exploreCmd)

	exploreCmd.SetHelpTemplate(getHelpTextExplore())
	exploreCmd.Flags().BoolP("list", "l", false, "export a list of the 'special' blocks")
	exploreCmd.Flags().BoolP("timestamps", "t", false, "ignore other options and generate timestamps only")
}

func getHelpTextExplore() string {
	debugText := ""
	if os.Getenv("TEST_MODE") == "true" {
		debugText = `chifra argc: 5 [1:explore] [2:--help] [3:--verbose] [4:2] 
chifra explore --help --verbose 2 
chifra explore argc: 4 [1:--help] [2:--verbose] [3:2] 
chifra explore --help --verbose 2 
PROG_NAME = [chifra explore]
`
	}
	helpText := `
  Usage:    chifra explore [-l|-g|-h] <term> [term...]  
  Purpose:  Open an explorer for one or more addresses, blocks, or transactions.

  Where:
    terms                 one or more addresses, names, block, or transaction identifiers
    -l  (--local)         open the local TrueBlocks explorer
    -g  (--google)        search google excluding popular blockchain explorers
    -h  (--help)          display this help screen

  Powered by TrueBlocks
`
	return debugText + helpText
}
