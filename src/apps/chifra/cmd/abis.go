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
	"errors"
	"os"

	"github.com/spf13/cobra"
	// EXISTING_CODE
)

// abisCmd represents the abis command
var abisCmd = &cobra.Command{
	Use:   usageAbis,
	Short: shortAbis,
	Long:  longAbis,
	Run:   runAbis,
	Args:  validateAbisArgs,
}

var usageAbis = `abis [flags] <address> [address...]

Arguments:
  addrs - a list of one or more smart contracts whose ABIs to display (required)`

var shortAbis = "fetches the ABI for a smart contract"

var longAbis = `Purpose:
  Fetches the ABI for a smart contract.`

var notesAbis = `
Notes:
  - Solidity files found in the local folder with the name '<address>.sol' are converted
    to an ABI prior to processing (and then removed).`

type abisOptionsType struct {
	known   bool
	sol     string
	find    []string
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
	abisCmd.Flags().StringSliceVarP(&AbisOpts.find, "find", "f", nil, "try to search for a function declaration given a four byte code")
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
	for _, t := range AbisOpts.find {
		options += " --find " + t
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
	// EXISTING_CODE
	// EXISTING_CODE
	PassItOn(GetCommandPath("grabABI"), options, arguments)
}

// EXISTING_CODE
// EXISTING_CODE

func validateAbisArgs(cmd *cobra.Command, args []string) error {
	var err error
	// EXISTING_CODE
	if AbisOpts.classes {
		return errors.New(fmtError("chifra abis - --classes option is not implemented."))
	}
	if len(AbisOpts.sol) > 0 {
		valid, _ := isValidAddress(AbisOpts.sol)
		if !valid {
			return errors.New(fmtError("--sol option requires a valid Ethereum address"))
		}
		if FileExists("./"+AbisOpts.sol+".sol") != true {
			return errors.New(fmtError("Solidity code not found at at ./" + AbisOpts.sol + ".sol"))
		}
		return nil
	}

	if len(AbisOpts.find) == 0 && AbisOpts.known == false {
		err = validateOneAddr(args)
		if err != nil {
			return err
		}
	}
	// EXISTING_CODE
	err = validateGlobalFlags(cmd, args)
	if err != nil {
		return err
	}
	return nil
}
