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
	Use:   "names [flags] <term> [term...]  ",
	Short: "query addresses or names of well known accounts",
	Long: `Purpose:
  Query addresses or names of well known accounts.`,
	Run: func(cmd *cobra.Command, args []string) {
		if RootOpts.oldHelp {
			fmt.Print(getHelpTextNames())
			return
		}
		fmt.Println("names called")
	},
	PostRun: PostNames,
}

func init() {
	rootCmd.AddCommand(namesCmd)

	// namesCmd.SetHelpTemplate(getHelpTextNames())
	namesCmd.Flags().SortFlags = false
	namesCmd.Flags().StringP("terms", "", "", "a space separated list of one or more search terms (required)")
	namesCmd.Flags().BoolP("expand", "e", false, "expand search to include all fields (default searches name, address, and symbol only)")
	namesCmd.Flags().BoolP("match_case", "m", false, "do case-sensitive search")
	namesCmd.Flags().BoolP("all", "l", false, "include all accounts in the search")
	namesCmd.Flags().BoolP("custom", "c", false, "include your custom named accounts")
	namesCmd.Flags().BoolP("prefund", "p", false, "include prefund accounts")
	namesCmd.Flags().BoolP("named", "n", false, "include well know token and airdrop addresses in the search")
	namesCmd.Flags().BoolP("addr", "a", false, "display only addresses in the results (useful for scripting)")
	namesCmd.Flags().BoolP("collections", "s", false, "display collections data")
	namesCmd.Flags().BoolP("tags", "g", false, "export the list of tags and subtags only")
	namesCmd.Flags().BoolP("to_custom", "u", false, "for editCmd only, is the edited name a custom name or not")
	namesCmd.Flags().BoolP("clean", "C", false, "clean the data (addrs to lower case, sort by addr)")
	namesCmd.Flags().StringP("autoname", "A", "", "an address assumed to be a token, added automatically to names database if true")
}

// func IsTestMode() bool {
// 	rootCmd.GetBool
// }

func getHelpTextNames() string {
	// namesCmd.DebugFlags()
	// return strconv.Itoa(Len(namesCmd))
	// return namesCmd.CommandPath() + " " + namesCmd.UseLine() + "\n"
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

    -u  (--to_custom)     for editCmd only, is the edited name a custom name or not
    -C  (--clean)         clean the data (addrs to lower case, sort by addr)
    -A  (--autoname <str>)an address assumed to be a token, added automatically to names database if true
`
}

func PostNames(cmd *cobra.Command, args []string) {
	fmt.Print(`
  Notes:
    - The tool will accept up to three terms, each of which must match against any field in the database.
    - The --match_case option enables case sensitive matching.

  Powered by TrueBlocks (GHC-TrueBlocks//0.12.1-alpha-7c5fb3f2a-20210923)
`)
}
