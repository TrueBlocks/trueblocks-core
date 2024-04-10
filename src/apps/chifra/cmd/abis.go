// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package cmd

// EXISTING_CODE
import (
	"os"

	abisPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/abis"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// abisCmd represents the abis command
var abisCmd = &cobra.Command{
	Use:     usageAbis,
	Long:    longAbis,
	Version: versionText,
	PreRun: outputHelpers.PreRunWithJsonWriter("abis", func() *globals.GlobalOptions {
		return &abisPkg.GetOptions().Globals
	}),
	RunE: file.RunWithFileSupport("abis", abisPkg.RunAbis, abisPkg.ResetOptions),
	PostRun: outputHelpers.PostRunWithJsonWriter(func() *globals.GlobalOptions {
		return &abisPkg.GetOptions().Globals
	}),
}

const usageAbis = `abis [flags] <address> [address...]

Arguments:
  addrs - a list of one or more smart contracts whose ABIs to display (required)`

const longAbis = `Purpose:
  Fetches the ABI for a smart contract.`

const notesAbis = `
Notes:
  - Search for either four byte signatures or event signatures with the --find option.`

func init() {
	var capabilities caps.Capability // capabilities for chifra abis
	capabilities = capabilities.Add(caps.Default)
	capabilities = capabilities.Add(caps.Caching)

	abisCmd.Flags().SortFlags = false

	abisCmd.Flags().BoolVarP(&abisPkg.GetOptions().Known, "known", "k", false, `load common 'known' ABIs from cache`)
	abisCmd.Flags().StringVarP(&abisPkg.GetOptions().ProxyFor, "proxy_for", "r", "", `redirects the query to this implementation`)
	abisCmd.Flags().StringSliceVarP(&abisPkg.GetOptions().Find, "find", "f", nil, `search for function or event declarations given a four- or 32-byte code(s)`)
	abisCmd.Flags().StringSliceVarP(&abisPkg.GetOptions().Hint, "hint", "n", nil, `for the --find option only, provide hints to speed up the search`)
	abisCmd.Flags().StringVarP(&abisPkg.GetOptions().Encode, "encode", "e", "", `generate the 32-byte encoding for a given cannonical function or event signature`)
	globals.InitGlobals("abis", abisCmd, &abisPkg.GetOptions().Globals, capabilities)

	abisCmd.SetUsageTemplate(UsageWithNotes(notesAbis))
	abisCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand(abisCmd)
}
