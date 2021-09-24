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

// namesCmd represents the names command
var namesCmd = &cobra.Command{
	Use: `names [flags] <term> [term...]

Arguments:
  terms - a space separated list of one or more search terms (required)`,
	Short: "query addresses or names of well known accounts",
	Long: `Purpose:
  Query addresses or names of well known accounts.`,
	Run: func(cmd *cobra.Command, args []string) {
		fmt.Println("names called")
	},
}

func init() {
	rootCmd.AddCommand(namesCmd)

	namesCmd.Flags().SortFlags = false
	namesCmd.PersistentFlags().SortFlags = false
	namesCmd.SetOut(os.Stderr)
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
	namesCmd.Flags().MarkHidden("terms")
}
