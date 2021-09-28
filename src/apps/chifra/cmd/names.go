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
	"os"

	"github.com/spf13/cobra"
	// EXISTING_CODE
)

// namesCmd represents the names command
var namesCmd = &cobra.Command{
	Use:   usageNames,
	Short: shortNames,
	Long:  longNames,
	Run:   runNames,
	Args:  validateNamesArgs,
}

var usageNames = `names [flags] <term> [term...]

Arguments:
  terms - a space separated list of one or more search terms (required)`

var shortNames = "query addresses or names of well known accounts"

var longNames = `Purpose:
  Query addresses or names of well known accounts.`

var notesNames = `
Notes:
  - The tool will accept up to three terms, each of which must match against any field in the database.
  - The --match_case option enables case sensitive matching.`

type namesOptionsType struct {
	expand      bool
	match_case  bool
	all         bool
	custom      bool
	prefund     bool
	named       bool
	addr        bool
	collections bool
	tags        bool
	to_custom   bool
	clean       bool
	autoname    string
}

var NamesOpts namesOptionsType

func init() {
	namesCmd.SetOut(os.Stderr)

	namesCmd.Flags().SortFlags = false
	namesCmd.PersistentFlags().SortFlags = false
	namesCmd.Flags().BoolVarP(&NamesOpts.expand, "expand", "e", false, "expand search to include all fields (default searches name, address, and symbol only)")
	namesCmd.Flags().BoolVarP(&NamesOpts.match_case, "match_case", "m", false, "do case-sensitive search")
	namesCmd.Flags().BoolVarP(&NamesOpts.all, "all", "l", false, "include all accounts in the search")
	namesCmd.Flags().BoolVarP(&NamesOpts.custom, "custom", "c", false, "include your custom named accounts")
	namesCmd.Flags().BoolVarP(&NamesOpts.prefund, "prefund", "p", false, "include prefund accounts")
	namesCmd.Flags().BoolVarP(&NamesOpts.named, "named", "n", false, "include well know token and airdrop addresses in the search")
	namesCmd.Flags().BoolVarP(&NamesOpts.addr, "addr", "a", false, "display only addresses in the results (useful for scripting)")
	namesCmd.Flags().BoolVarP(&NamesOpts.collections, "collections", "s", false, "display collections data")
	namesCmd.Flags().BoolVarP(&NamesOpts.tags, "tags", "g", false, "export the list of tags and subtags only")
	namesCmd.Flags().BoolVarP(&NamesOpts.to_custom, "to_custom", "u", false, "for editCmd only, is the edited name a custom name or not (hidden)")
	namesCmd.Flags().BoolVarP(&NamesOpts.clean, "clean", "C", false, "clean the data (addrs to lower case, sort by addr) (hidden)")
	namesCmd.Flags().StringVarP(&NamesOpts.autoname, "autoname", "A", "", "an address assumed to be a token, added automatically to names database if true (hidden)")
	if IsTestMode() == false {
		namesCmd.Flags().MarkHidden("to_custom")
		namesCmd.Flags().MarkHidden("clean")
		namesCmd.Flags().MarkHidden("autoname")
	}
	namesCmd.Flags().SortFlags = false
	namesCmd.PersistentFlags().SortFlags = false

	namesCmd.SetUsageTemplate(HelpWithNotes(notesNames))
	rootCmd.AddCommand(namesCmd)
}

func runNames(cmd *cobra.Command, args []string) {
	options := ""
	if NamesOpts.expand {
		options += " --expand"
	}
	if NamesOpts.match_case {
		options += " --match_case"
	}
	if NamesOpts.all {
		options += " --all"
	}
	if NamesOpts.custom {
		options += " --custom"
	}
	if NamesOpts.prefund {
		options += " --prefund"
	}
	if NamesOpts.named {
		options += " --named"
	}
	if NamesOpts.addr {
		options += " --addr"
	}
	if NamesOpts.collections {
		options += " --collections"
	}
	if NamesOpts.tags {
		options += " --tags"
	}
	if NamesOpts.to_custom {
		options += " --to_custom"
	}
	if NamesOpts.clean {
		options += " --clean"
	}
	if len(NamesOpts.autoname) > 0 {
		options += " --autoname " + NamesOpts.autoname
	}
	arguments := ""
	for _, arg := range args {
		arguments += " " + arg
	}
	// EXISTING_CODE
	// EXISTING_CODE
	PassItOn(GetCommandPath("ethNames"), options, arguments)
}

// EXISTING_CODE
// EXISTING_CODE

func validateNamesArgs(cmd *cobra.Command, args []string) error {
	var err error
	// EXISTING_CODE
	// EXISTING_CODE
	err = validateGlobalFlags(cmd, args)
	if err != nil {
		return err
	}
	return nil
}
