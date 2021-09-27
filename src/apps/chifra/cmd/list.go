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
 * Parts of this file were generated with makeClass --gocmds.
 */

import (
	"errors"
	"os"

	"github.com/spf13/cobra"
)

// listCmd represents the list command
var listCmd = &cobra.Command{
	Use:   usageList,
	Short: shortList,
	Long:  longList,
	Run:   runList,
	Args:  ValidatePositionals(validateListArgs),
}

var usageList = `list [flags] <address> [address...]

Arguments:
  addrs - one or more addresses (0x...) to list (required)`

var shortList = "list every appearance of an address anywhere on the chain"

var longList = `Purpose:
  List every appearance of an address anywhere on the chain.`

var notesList = ``

type listOptionsType struct {
	appearances bool
	count       bool
}

var ListOpts listOptionsType

func init() {
	listCmd.SetOut(os.Stderr)

	listCmd.Flags().SortFlags = false
	listCmd.PersistentFlags().SortFlags = false
	listCmd.Flags().BoolVarP(&ListOpts.appearances, "appearances", "p", false, "export a list of appearances")
	listCmd.Flags().BoolVarP(&ListOpts.count, "count", "U", false, "return only the number of records")
	listCmd.Flags().SortFlags = false
	listCmd.PersistentFlags().SortFlags = false

	listCmd.SetUsageTemplate(HelpWithNotes(notesList))
	rootCmd.AddCommand(listCmd)
}

func runList(cmd *cobra.Command, args []string) {
	options := ""
	// if ListOpts.appearances {
	options += " --appearances"
	// }
	if ListOpts.count {
		options += " --count"
	}
	arguments := ""
	for _, arg := range args {
		arguments += " " + arg
	}
	PassItOn("/Users/jrush/.local/bin/chifra/acctExport", options, arguments)
}

func validateListArgs(cmd *cobra.Command, args []string) error {
	if len(args) > 0 && args[0] == "12" {
		return ErrFunc(cmd, errors.New("Invalid argument "+args[0]))
	}
	return nil
}
