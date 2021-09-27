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

// abisCmd represents the abis command
var abisCmd = &cobra.Command{
	Use:   usageAbis,
	Short: shortAbis,
	Long:  longAbis,
	Run:   runAbis,
	Args:  ValidatePositionals(validateAbisArgs),
}

var usageAbis = `abis [flags] <address> [address...]

Arguments:
  addrs - list of one or more smart contracts whose ABI to grab from EtherScan (required)`

var shortAbis = "fetches the ABI for a smart contract"

var longAbis = `Purpose:
  Fetches the ABI for a smart contract.`

var notesAbis = `
Notes:
  - Solidity files found in the local folder with the name '<address>.sol' are converted to an ABI prior to processing (and then removed).`

type abisOptionsType struct {
	known   bool
	sol     string
	find    string
	source  bool
	classes bool
}

var AbisOpts abisOptionsType

func init() {
	abisCmd.SetOut(os.Stderr)

	abisCmd.Flags().SortFlags = false
	abisCmd.PersistentFlags().SortFlags = false
	abisCmd.Flags().BoolVarP(&AbisOpts.known, "known", "k", false, "load common 'known' ABIs from cache")
	abisCmd.Flags().StringVarP(&AbisOpts.sol, "sol", "s", "", "file name of .sol file from which to create a new known abi (without .sol)")
	abisCmd.Flags().StringVarP(&AbisOpts.find, "find", "f", "", "try to search for a function declaration given a four byte code")
	abisCmd.Flags().BoolVarP(&AbisOpts.source, "source", "o", false, "show the source of the ABI information (hidden)")
	abisCmd.Flags().BoolVarP(&AbisOpts.classes, "classes", "c", false, "generate classDefinitions folder and class definitions (hidden)")
	if IsTestMode() == false {
		abisCmd.Flags().MarkHidden("source")
		abisCmd.Flags().MarkHidden("classes")
	}
	abisCmd.Flags().SortFlags = false
	abisCmd.PersistentFlags().SortFlags = false

	abisCmd.SetUsageTemplate(HelpWithNotes(notesAbis))
	rootCmd.AddCommand(abisCmd)
}

func runAbis(cmd *cobra.Command, args []string) {
	options := ""
	if AbisOpts.known {
		options += " --known"
	}
	if len(AbisOpts.sol) > 0 {
		options += " --sol " + AbisOpts.sol
	}
	if len(AbisOpts.find) > 0 {
		options += " --find " + AbisOpts.find
	}
	if AbisOpts.source {
		options += " --source"
	}
	if AbisOpts.classes {
		options += " --classes"
	}
	arguments := ""
	for _, arg := range args {
		arguments += " " + arg
	}
	PassItOn("/Users/jrush/.local/bin/chifra/grabABI", options, arguments)
}

func validateAbisArgs(cmd *cobra.Command, args []string) error {
	if len(args) > 0 && args[0] == "12" {
		return ErrFunc(cmd, errors.New("Invalid argument "+args[0]))
	}
	return nil
}
