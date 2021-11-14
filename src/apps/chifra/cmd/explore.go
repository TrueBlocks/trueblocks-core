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
 * The file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */
package cmd

import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/cmd/globals"
	explorePkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/explore"
	"github.com/spf13/cobra"
)

// exploreCmd represents the explore command
var exploreCmd = &cobra.Command{
	Use:   usageExplore,
	Short: shortExplore,
	Long:  longExplore,
	RunE:  explorePkg.Run,
	Args:  explorePkg.Validate,
}

var usageExplore = `explore [flags] <term> [term...]

Arguments:
  terms - one or more address, name, block, or transaction identifier`

var shortExplore = "open a local or remote explorer for one or more addresses, blocks, or transactions"

var longExplore = `Purpose:
  Open a local or remote explorer for one or more addresses, blocks, or transactions.`

var notesExplore = ``

func init() {
	exploreCmd.Flags().SortFlags = false
	exploreCmd.Flags().BoolVarP(&explorePkg.Options.Local, "local", "l", false, "open the local TrueBlocks explorer")
	exploreCmd.Flags().BoolVarP(&explorePkg.Options.Google, "google", "g", false, "search google excluding popular blockchain explorers")
	exploreCmd.Flags().SortFlags = false
	globals.GlobalOptions(exploreCmd, &explorePkg.Options.Globals)

	exploreCmd.SetUsageTemplate(UsageWithNotes(notesExplore))
	exploreCmd.SetOut(os.Stderr)
	if explorePkg.Options.Globals.ApiMode {
		exploreCmd.SetErr(os.Stdout)
	}
	chifraCmd.AddCommand(exploreCmd)
}
