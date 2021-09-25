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
	"os"

	"github.com/spf13/cobra"
)

type exploreOptionsType struct {
	local  bool
	google bool
}

var ExploreOpts exploreOptionsType

// exploreCmd represents the explore command
var exploreCmd = &cobra.Command{
	Use: `explore [flags] <term> [term...]

Arguments:
  terms - one or more addresses, names, block, or transaction identifiers`,
	Short: "open an explorer for a given address, block, or transaction",
	Long: `Purpose:
  Open an explorer for a given address, block, or transaction.`,
	Run: runExplore,
}

func init() {
	exploreCmd.SetOut(os.Stderr)

	exploreCmd.Flags().SortFlags = false
	exploreCmd.PersistentFlags().SortFlags = false
	exploreCmd.Flags().BoolVarP(&ExploreOpts.local, "local", "l", false, "open the local TrueBlocks explorer")
	exploreCmd.Flags().BoolVarP(&ExploreOpts.google, "google", "g", false, "search google excluding popular blockchain explorers")
	exploreCmd.Flags().SortFlags = false
	exploreCmd.PersistentFlags().SortFlags = false

	rootCmd.AddCommand(exploreCmd)
}

func runExplore(cmd *cobra.Command, args []string) {
	options := ""
	if ExploreOpts.local {
		options += " --local"
	}
	if ExploreOpts.google {
		options += " --google"
	}
	arguments := ""
    for _, arg := range args {
		arguments += " " + arg
	}
	PassItOn("/Users/jrush/.local/bin/chifra/fireStorm", options, arguments)
}
