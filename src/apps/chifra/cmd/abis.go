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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/cmd/abis"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/spf13/cobra"
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
  - For the --sol option, place the solidity files in the current working folder.
  - Search for either four byte signatures or event signatures with the --find option.`

var AbisOpts abis.AbisOptionsType

func init() {
	if utils.IsApiMode() {
		abisCmd.SetOut(os.Stderr)
		abisCmd.SetErr(os.Stdout)
	} else {
		abisCmd.SetOut(os.Stderr)
	}

	abisCmd.Flags().SortFlags = false
	abisCmd.PersistentFlags().SortFlags = false
	abisCmd.Flags().BoolVarP(&AbisOpts.Known, "known", "k", false, "load common 'known' ABIs from cache")
	abisCmd.Flags().BoolVarP(&AbisOpts.Sol, "sol", "s", false, "extract the abi definition from the provided .sol file(s)")
	abisCmd.Flags().StringSliceVarP(&AbisOpts.Find, "find", "f", nil, "search for function or event declarations given a four- or 32-byte code(s)")
	abisCmd.Flags().BoolVarP(&AbisOpts.Source, "source", "o", false, "show the source of the ABI information (hidden)")
	abisCmd.Flags().BoolVarP(&AbisOpts.Classes, "classes", "c", false, "generate classDefinitions folder and class definitions (hidden)")
	if !utils.IsTestMode() {
		abisCmd.Flags().MarkHidden("source")
		abisCmd.Flags().MarkHidden("classes")
	}
	abisCmd.Flags().SortFlags = false
	abisCmd.PersistentFlags().SortFlags = false

	abisCmd.SetUsageTemplate(UsageWithNotes(notesAbis))
	rootCmd.AddCommand(abisCmd)
}

func TestLogAbis(args []string) {
	if !utils.IsTestMode() {
		return
	}
}

// EXISTING_CODE
// EXISTING_CODE
