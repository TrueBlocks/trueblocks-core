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

// namesCmd represents the names command
var namesCmd = &cobra.Command{
	Use:   "names",
	Short: "A brief description of your command",
	Long: `A longer description that spans multiple lines and likely contains examples
and usage of using your command. For example:

Cobra is a CLI library for Go that empowers applications.
This application is a tool to generate the needed files
to quickly create a Cobra application.`,
	Run: func(cmd *cobra.Command, args []string) {
		fmt.Println("names called")
	},
}

func init() {
	rootCmd.AddCommand(namesCmd)
	namesCmd.SetHelpTemplate(getHelpTextNames())
}

func getHelpTextNames() string {
	return `chifra argc: 5 [1:names] [2:--help] [3:--verbose] [4:2] 
chifra names --help --verbose 2 
chifra names argc: 4 [1:--help] [2:--verbose] [3:2] 
chifra names --help --verbose 2 
PROG_NAME = [chifra names]

  Usage:    chifra names [-e|-m|-l|-c|-p|-n|-a|-s|-g|-v|-h] <term> [term...]  
  Purpose:  Query addresses or names of well known accounts.

  Where:
    terms                 a space separated list of one or more search terms (required)
    -e  (--expand)        expand search to include all fields (default searches name, address, and symbol only)
    -m  (--match_case)    do case-sensitive search
    -l  (--all)           include all accounts in the search
    -c  (--custom)        include your custom named accounts
    -p  (--prefund)       include prefund accounts
    -n  (--named)         include well know token and airdrop addresses in the search
    -a  (--addr)          display only addresses in the results (useful for scripting)
    -s  (--collections)   display collections data
    -g  (--tags)          export the list of tags and subtags only

    #### Hidden options
    -u  (--to_custom)     for editCmd only, is the edited name a custom name or not
    -C  (--clean)         clean the data (addrs to lower case, sort by addr)
    -A  (--autoname <str>)an address assumed to be a token, added automatically to names database if true
    #### Hidden options

    -x  (--fmt <val>)     export format, one of [none|json*|txt|csv|api]
    -v  (--verbose)       set verbose level (optional level defaults to 1)
    -h  (--help)          display this help screen

  Notes:
    - The tool will accept up to three terms, each of which must match against any field in the database.
    - The --match_case option enables case sensitive matching.

  Powered by TrueBlocks
`
}
