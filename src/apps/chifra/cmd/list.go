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
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

// EXISTING_CODE
import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	listPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/list"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// listCmd represents the list command
var listCmd = &cobra.Command{
	Use:     usageList,
	Short:   shortList,
	Long:    longList,
	Version: "GHC-TrueBlocks//0.17.0-alpha",
	RunE:    listPkg.RunList,
}

var usageList = `list [flags] <address> [address...]

Arguments:
  addrs - one or more addresses (0x...) to list (required)`

var shortList = "list every appearance of an address anywhere on the chain"

var longList = `Purpose:
  List every appearance of an address anywhere on the chain.`

var notesList = ``

func init() {
	listCmd.Flags().SortFlags = false

	listCmd.Flags().BoolVarP(&listPkg.Options.Count, "count", "U", false, "present only the number of records")
	listCmd.Flags().BoolVarP(&listPkg.Options.Appearances, "appearances", "p", false, "export a list of appearances (hidden)")
	listCmd.Flags().Uint64VarP(&listPkg.Options.FirstBlock, "first_block", "F", 0, "first block to process (inclusive) (hidden)")
	listCmd.Flags().Uint64VarP(&listPkg.Options.LastBlock, "last_block", "L", 0, "last block to process (inclusive) (hidden)")
	if os.Getenv("TEST_MODE") != "true" {
		listCmd.Flags().MarkHidden("appearances")
		listCmd.Flags().MarkHidden("first_block")
		listCmd.Flags().MarkHidden("last_block")
	}
	globals.InitGlobals(listCmd, &listPkg.Options.Globals)

	listCmd.SetUsageTemplate(UsageWithNotes(notesList))
	listCmd.SetOut(os.Stderr)

	chifraCmd.AddCommand(listCmd)
}
