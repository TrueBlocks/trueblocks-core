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

	listPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/cmd/list"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/spf13/cobra"
)

// listCmd represents the list command
var listCmd = &cobra.Command{
	Use:   usageList,
	Short: shortList,
	Long:  longList,
	RunE:  listPkg.Run,
	Args:  listPkg.Validate,
}

var usageList = `list [flags] <address> [address...]

Arguments:
  addrs - one or more addresses (0x...) to list (required)`

var shortList = "list every appearance of an address anywhere on the chain"

var longList = `Purpose:
  List every appearance of an address anywhere on the chain.`

var notesList = ``

func init() {
	listCmd.SetOut(os.Stderr)

	listCmd.Flags().SortFlags = false
	listCmd.PersistentFlags().SortFlags = false
	listCmd.Flags().BoolVarP(&listPkg.Options.Count, "count", "U", false, "present only the number of records")
	listCmd.Flags().BoolVarP(&listPkg.Options.Appearances, "appearances", "p", false, "export a list of appearances (hidden)")
	listCmd.Flags().Uint64VarP(&listPkg.Options.FirstBlock, "first_block", "F", 0, "first block to process (inclusive) (hidden)")
	listCmd.Flags().Uint64VarP(&listPkg.Options.LastBlock, "last_block", "L", 0, "last block to process (inclusive) (hidden)")
	if !utils.IsTestMode() {
		listCmd.Flags().MarkHidden("appearances")
		listCmd.Flags().MarkHidden("first_block")
		listCmd.Flags().MarkHidden("last_block")
	}
	listCmd.Flags().SortFlags = false
	listCmd.PersistentFlags().SortFlags = false

	listCmd.SetUsageTemplate(UsageWithNotes(notesList))
	rootCmd.AddCommand(listCmd)
}
