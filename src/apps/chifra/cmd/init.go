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
	initPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/init"
	"github.com/spf13/cobra"
)

// initCmd represents the init command
var initCmd = &cobra.Command{
	Use:   usageInit,
	Short: shortInit,
	Long:  longInit,
	RunE:  initPkg.RunInit,
}

var usageInit = `init [flags]`

var shortInit = "initialize the TrueBlocks system by downloading from IPFS"

var longInit = `Purpose:
  Initialize the TrueBlocks system by downloading from IPFS.`

var notesInit = `
Notes:
  - chifra init is an alias for the chifra pins --init command.
  - See chifra pins --help for more information.`

func init() {
	initCmd.Flags().SortFlags = false

	initCmd.Flags().BoolVarP(&initPkg.Options.All, "all", "a", false, "in addition to Bloom filters, download full index chunks")
	globals.GlobalOptions(initCmd, &initPkg.Options.Globals)

	initCmd.SetUsageTemplate(UsageWithNotes(notesInit))

	initCmd.SetOut(os.Stderr)
	if initPkg.Options.Globals.ApiMode {
		initCmd.SetErr(os.Stdout)
	}

	chifraCmd.AddCommand(initCmd)
}
