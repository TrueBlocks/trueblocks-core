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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	slurpPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/slurp"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// slurpCmd represents the slurp command
var slurpCmd = &cobra.Command{
	Use:     usageSlurp,
	Long:    longSlurp,
	Version: versionText,
	PreRun: outputHelpers.PreRunWithJsonWriter("slurp", func() *globals.GlobalOptions {
		return &slurpPkg.GetOptions().Globals
	}),
	RunE: file.RunWithFileSupport("slurp", slurpPkg.RunSlurp, slurpPkg.ResetOptions),
	PostRun: outputHelpers.PostRunWithJsonWriter(func() *globals.GlobalOptions {
		return &slurpPkg.GetOptions().Globals
	}),
}

const usageSlurp = `slurp [flags] <address> [address...] [block...]

Arguments:
  addrs - one or more addresses to slurp from Etherscan (required)
  blocks - an optional range of blocks to slurp`

const longSlurp = `Purpose:
  Fetch data from Etherscan and other APIs for any address.`

const notesSlurp = `
Notes:
  - An address must be either an ENS name or start with '0x' and be forty-two characters long.
  - Portions of this software are Powered by Etherscan.io, Covalent, Alchemy, TrueBlocks Key APIs.
  - See slurp/README on how to configure keys for API providers.
  - The withdrawals option is only available on certain chains. It is ignored otherwise.
  - If the value of --source is key, --parts is ignored.
  - The --types option is deprecated, use --parts instead.`

func init() {
	var capabilities caps.Capability // capabilities for chifra slurp
	capabilities = capabilities.Add(caps.Default)
	capabilities = capabilities.Add(caps.Caching)
	capabilities = capabilities.Add(caps.Ether)

	slurpCmd.Flags().SortFlags = false

	slurpCmd.Flags().StringSliceVarP(&slurpPkg.GetOptions().Parts, "parts", "r", nil, `which types of transactions to request
One or more of [ ext | int | token | nfts | 1155 | miner | uncles | withdrawals | some | all ]`)
	slurpCmd.Flags().BoolVarP(&slurpPkg.GetOptions().Appearances, "appearances", "p", false, `show only the blocknumber.tx_id appearances of the exported transactions`)
	slurpCmd.Flags().BoolVarP(&slurpPkg.GetOptions().Articulate, "articulate", "a", false, `articulate the retrieved data if ABIs can be found`)
	slurpCmd.Flags().StringVarP(&slurpPkg.GetOptions().Source, "source", "S", "etherscan", `the source of the slurped data
One of [ etherscan | key | covalent | alchemy ]`)
	slurpCmd.Flags().BoolVarP(&slurpPkg.GetOptions().Count, "count", "U", false, `for --appearances mode only, display only the count of records`)
	slurpCmd.Flags().Uint64VarP(&slurpPkg.GetOptions().Page, "page", "g", 0, `the page to retrieve (page number)`)
	slurpCmd.Flags().StringVarP(&slurpPkg.GetOptions().PageId, "page_id", "", "", `the page to retrieve (page ID)`)
	slurpCmd.Flags().Uint64VarP(&slurpPkg.GetOptions().PerPage, "per_page", "P", 1000, `the number of records to request on each page`)
	slurpCmd.Flags().Float64VarP(&slurpPkg.GetOptions().Sleep, "sleep", "s", .25, `seconds to sleep between requests`)
	slurpCmd.Flags().StringSliceVarP(&slurpPkg.GetOptions().Types, "types", "t", nil, `deprecated, use --parts instead (hidden)`)
	if os.Getenv("TEST_MODE") != "true" {
		_ = slurpCmd.Flags().MarkHidden("types")
	}
	_ = slurpCmd.Flags().MarkDeprecated("types", "The --types option has been deprecated.")
	globals.InitGlobals("slurp", slurpCmd, &slurpPkg.GetOptions().Globals, capabilities)

	slurpCmd.SetUsageTemplate(UsageWithNotes(notesSlurp))
	slurpCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand(slurpCmd)
}
