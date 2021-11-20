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

	abisPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/abis"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// abisCmd represents the abis command
var abisCmd = &cobra.Command{
	Use:     usageAbis,
	Short:   shortAbis,
	Long:    longAbis,
	Version: "GHC-TrueBlocks//0.16.1-alpha",
	RunE:    abisPkg.RunAbis,
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

func init() {
	abisCmd.Flags().SortFlags = false

	abisCmd.Flags().BoolVarP(&abisPkg.Options.Known, "known", "k", false, "load common 'known' ABIs from cache")
	abisCmd.Flags().BoolVarP(&abisPkg.Options.Sol, "sol", "s", false, "extract the abi definition from the provided .sol file(s)")
	abisCmd.Flags().StringSliceVarP(&abisPkg.Options.Find, "find", "f", nil, "search for function or event declarations given a four- or 32-byte code(s)")
	abisCmd.Flags().BoolVarP(&abisPkg.Options.Source, "source", "o", false, "show the source of the ABI information (hidden)")
	abisCmd.Flags().BoolVarP(&abisPkg.Options.Classes, "classes", "c", false, "generate classDefinitions folder and class definitions (hidden)")
	if os.Getenv("TEST_MODE") != "true" {
		abisCmd.Flags().MarkHidden("source")
		abisCmd.Flags().MarkHidden("classes")
	}
	globals.InitGlobals(abisCmd, &abisPkg.Options.Globals)

	abisCmd.SetUsageTemplate(UsageWithNotes(notesAbis))
	abisCmd.SetOut(os.Stderr)

	chifraCmd.AddCommand(abisCmd)
}
