// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package cmd

// EXISTING_CODE
import (
	"os"

	abisPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/abis"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// abisCmd represents the abis command
var abisCmd = &cobra.Command{
	Use:     usageAbis,
	Short:   shortAbis,
	Long:    longAbis,
	Version: versionText,
	PreRun: outputHelpers.PreRunWithJsonWriter("abis", func() *globals.GlobalOptions {
		return &abisPkg.GetOptions().Globals
	}),
	RunE:    file.RunWithFileSupport("abis", abisPkg.RunAbis, abisPkg.ResetOptions),
	PostRun: outputHelpers.PostRunWithJsonWriter(func() *globals.GlobalOptions {
		return &abisPkg.GetOptions().Globals
	}),
}

const usageAbis = `abis [flags] <address> [address...]

Arguments:
  addrs - a list of one or more smart contracts whose ABIs to display (required)`

const shortAbis = "fetches the ABI for a smart contract"

const longAbis = `Purpose:
  Fetches the ABI for a smart contract.`

const notesAbis = `
Notes:
  - For the --sol option, place the solidity files in the current working folder.
  - Search for either four byte signatures or event signatures with the --find option.`

func init() {
	abisCmd.Flags().SortFlags = false

	abisCmd.Flags().BoolVarP(&abisPkg.GetOptions().Known, "known", "k", false, "load common 'known' ABIs from cache")
	abisCmd.Flags().BoolVarP(&abisPkg.GetOptions().Sol, "sol", "s", false, "extract the abi definition from the provided .sol file(s)")
	abisCmd.Flags().StringSliceVarP(&abisPkg.GetOptions().Find, "find", "f", nil, "search for function or event declarations given a four- or 32-byte code(s)")
	abisCmd.Flags().StringSliceVarP(&abisPkg.GetOptions().Hint, "hint", "n", nil, "for the --find option only, provide hints to speed up the search")
	globals.InitGlobals(abisCmd, &abisPkg.GetOptions().Globals)

	abisCmd.SetUsageTemplate(UsageWithNotes(notesAbis))
	abisCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand(abisCmd)
}
