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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	tokensPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/tokens"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// tokensCmd represents the tokens command
var tokensCmd = &cobra.Command{
	Use:     usageTokens,
	Short:   shortTokens,
	Long:    longTokens,
	Version: versionText,
	RunE:    tokensPkg.RunTokens,
}

var usageTokens = `tokens [flags] <address> <address> [address...] [block...]

Arguments:
  addrs - two or more addresses (0x...), the first is an ERC20 token, balances for the rest are reported (required)
  blocks - an optional list of one or more blocks at which to report balances, defaults to 'latest'`

var shortTokens = "retrieve token balance(s) for one or more addresses at given block(s)"

var longTokens = `Purpose:
  Retrieve token balance(s) for one or more addresses at given block(s).`

var notesTokens = `
Notes:
  - An address must start with '0x' and be forty-two characters long.
  - blocks may be a space-separated list of values, a start-end range, a special, or any combination.
  - If the token contract(s) from which you request balances are not ERC20 compliant, the results are undefined.
  - If the queried node does not store historical state, the results are undefined.
  - special blocks are detailed under chifra when --list.`

func init() {
	tokensCmd.Flags().SortFlags = false

	tokensCmd.Flags().StringSliceVarP(&tokensPkg.GetOptions().Parts, "parts", "p", nil, `which parts of the token information to retrieve
One or more of [ name | symbol | decimals | totalSupply | version | none | all ]`)
	tokensCmd.Flags().BoolVarP(&tokensPkg.GetOptions().ByAcct, "by_acct", "b", false, "consider each address an ERC20 token except the last, whose balance is reported for each token")
	tokensCmd.Flags().BoolVarP(&tokensPkg.GetOptions().NoZero, "no_zero", "n", false, "suppress the display of zero balance accounts")
	globals.InitGlobals(tokensCmd, &tokensPkg.GetOptions().Globals)

	tokensCmd.SetUsageTemplate(UsageWithNotes(notesTokens))
	tokensCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand(tokensCmd)
}
