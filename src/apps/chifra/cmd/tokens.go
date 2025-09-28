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
	tokensPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/tokens"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// tokensCmd represents the tokens command
var tokensCmd = &cobra.Command{
	Use:     usageTokens,
	Long:    longTokens,
	Version: versionText,
	PreRun: outputHelpers.PreRunWithJsonWriter("tokens", func() *globals.GlobalOptions {
		return &tokensPkg.GetOptions().Globals
	}),
	RunE: file.RunWithFileSupport("tokens", tokensPkg.RunTokens, tokensPkg.ResetOptions),
	PostRun: outputHelpers.PostRunWithJsonWriter(func() *globals.GlobalOptions {
		return &tokensPkg.GetOptions().Globals
	}),
}

const usageTokens = `tokens [flags] <address> <address> [address...] [block...]

Arguments:
  addrs - two or more addresses (one for --approvals), the first is an ERC20 token, balances for the rest are reported (required)
  blocks - an optional list of one or more blocks at which to report balances, defaults to 'latest'`

const longTokens = `Purpose:
  Retrieve token balance(s) for one or more addresses at given block(s).`

const notesTokens = `
Notes:
  - An address must be either an ENS name or start with '0x' and be forty-two characters long.
  - Blocks is a space-separated list of values, a start-end range, a special, or any combination.
  - If the token contract(s) from which you request balances are not ERC20 compliant, the results are undefined.
  - If the queried node does not store historical state, the results are undefined.
  - Special blocks are detailed under chifra when --list.
  - If the --parts option is not empty, all addresses are considered tokens and each token's attributes are presented.
  - For the --approvals option, all open approvals are reported for each provided address.`

func init() {
	var capabilities caps.Capability // capabilities for chifra tokens
	capabilities = capabilities.Add(caps.Default)
	capabilities = capabilities.Add(caps.Caching)
	capabilities = capabilities.Add(caps.Names)

	tokensCmd.Flags().SortFlags = false

	tokensCmd.Flags().BoolVarP(&tokensPkg.GetOptions().Approvals, "approvals", "O", false, `returns all open approvals for the given address(es)`)
	tokensCmd.Flags().StringSliceVarP(&tokensPkg.GetOptions().Parts, "parts", "p", nil, `which parts of the token information to retrieve
One or more of [ name | symbol | decimals | totalSupply | version | some | all ]`)
	tokensCmd.Flags().BoolVarP(&tokensPkg.GetOptions().ByAcct, "by_acct", "b", false, `consider each address an ERC20 token except the last, whose balance is reported for each token`)
	tokensCmd.Flags().BoolVarP(&tokensPkg.GetOptions().Changes, "changes", "c", false, `only report a balance when it changes from one block to the next`)
	tokensCmd.Flags().BoolVarP(&tokensPkg.GetOptions().NoZero, "no_zero", "z", false, `suppress the display of zero balance accounts`)
	globals.InitGlobals("tokens", tokensCmd, &tokensPkg.GetOptions().Globals, capabilities)

	tokensCmd.SetUsageTemplate(UsageWithNotes(notesTokens))
	tokensCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand(tokensCmd)
}
