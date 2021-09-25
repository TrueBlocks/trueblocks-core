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

type initOptionsType struct {
}

var InitOpts initOptionsType

// initCmd represents the init command
var initCmd = &cobra.Command{
	Use: `init [flags]`,
	Short: "initialize the index of appearances by downloading Bloom filters",
	Long: `Purpose:
  Initialize the index of appearances by downloading Bloom filters.`,
	Run: runInit,
}

func init() {
	initCmd.SetOut(os.Stderr)

	initCmd.Flags().SortFlags = false
	initCmd.PersistentFlags().SortFlags = false
	initCmd.Flags().SortFlags = false
	initCmd.PersistentFlags().SortFlags = false

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
