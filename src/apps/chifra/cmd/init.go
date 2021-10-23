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
package cmd

import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/utils"
	"github.com/spf13/cobra"
)

// initCmd represents the init command
var initCmd = &cobra.Command{
	Use:   usageInit,
	Short: shortInit,
	Long:  longInit,
	Run:   runInit,
	Args:  validateInitArgs,
}

var usageInit = `init [flags]`

var shortInit = "initialize the TrueBlocks system by downloading from IPFS"

var longInit = `Purpose:
  Initialize the TrueBlocks system by downloading from IPFS.`

var notesInit = `
Notes:
  - chifra init is an alias for the chifra pins --init command.
  - See chifra pins --help for more information.`

type initOptionsType struct {
	all bool
}

var InitOpts initOptionsType

func init() {
	initCmd.SetOut(os.Stderr)

	initCmd.Flags().SortFlags = false
	initCmd.PersistentFlags().SortFlags = false
	initCmd.Flags().BoolVarP(&InitOpts.all, "all", "a", false, "in addition to Bloom filters, download full index chunks")
	initCmd.Flags().SortFlags = false
	initCmd.PersistentFlags().SortFlags = false

	initCmd.SetUsageTemplate(UsageWithNotes(notesInit))
	rootCmd.AddCommand(initCmd)
}

func TestLogInit(args []string) {
	if !utils.IsTestMode() {
		return
	}
}

// EXISTING_CODE
// EXISTING_CODE
