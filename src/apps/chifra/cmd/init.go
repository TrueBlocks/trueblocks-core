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

// initCmd represents the init command
var initCmd = &cobra.Command{
	Use:   usageInit,
	Short: shortInit,
	Long:  longInit,
	Run:   runInit,
	Args:  validateInitArgs,
}

var usageInit = `init [flags]`

var shortInit = "initialize the index of appearances by downloading Bloom filters"

var longInit = `Purpose:
  Initialize the index of appearances by downloading Bloom filters.`

var notesInit = ``

type initOptionsType struct {
}

var InitOpts initOptionsType

func init() {
	initCmd.SetOut(os.Stderr)

	initCmd.Flags().SortFlags = false
	initCmd.PersistentFlags().SortFlags = false
	initCmd.Flags().SortFlags = false
	initCmd.PersistentFlags().SortFlags = false

	initCmd.SetUsageTemplate(HelpWithNotes(notesInit))
	rootCmd.AddCommand(initCmd)
}

func runInit(cmd *cobra.Command, args []string) {
	options := ""
	arguments := ""
	for _, arg := range args {
		arguments += " " + arg
	}
	// EXISTING_CODE
	// EXISTING_CODE
	PassItOn(GetCommandPath("pinMan local --init"), options, arguments)
}

// EXISTING_CODE
// EXISTING_CODE

func validateInitArgs(cmd *cobra.Command, args []string) error {
	var err error
	// EXISTING_CODE
	// EXISTING_CODE
	err = validateGlobalFlags(cmd, args)
	if err != nil {
		return err
	}
	return nil
}
