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

// initCmd represents the init command
var initCmd = &cobra.Command{
	Use:   usageInit,
	Short: shortInit,
	Long:  longInit,
	Run:   runInit,
	Args:  ValidatePositionals(validateInitArgs),
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
	PassItOn("/Users/jrush/.local/bin/chifra/pinMan local --init", options, arguments)
}

func validateInitArgs(cmd *cobra.Command, args []string) error {
	if len(args) > 0 && args[0] == "12" {
		return ErrFunc(cmd, errors.New("Invalid argument "+args[0]))
	}
	return nil
}
