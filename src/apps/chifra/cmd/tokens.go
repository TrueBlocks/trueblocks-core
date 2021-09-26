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

// tokensCmd represents the tokens command
var tokensCmd = &cobra.Command{
	Use:   usageTokens,
	Short: shortTokens,
	Long:  longTokens,
	Run:   runTokens,
	Args:  ValidatePositionals(validateTokensArgs, cobra.MinimumNArgs(1)),
}

var usageTokens = `tokens [flags] <address> <address> [address...] [block...]

Arguments:
  addrs - two or more addresses (0x...), the first is an ERC20 token, balances for the rest are reported (required)
  blocks - an optional list of one or more blocks at which to report balances, defaults to 'latest'`

var shortTokens = "retrieve token balance(s) for one or more addresses at given block(s)"

var longTokens = `Purpose:
  Retrieve token balance(s) for one or more addresses at given block(s).`

type tokensOptionsType struct {
	parts   string
	by_acct bool
	no_zero bool
}

var TokensOpts tokensOptionsType

func init() {
	tokensCmd.SetOut(os.Stderr)

	tokensCmd.Flags().SortFlags = false
	tokensCmd.PersistentFlags().SortFlags = false
	tokensCmd.Flags().StringVarP(&TokensOpts.parts, "parts", "p", "", "one or more parts of the token information to retreive")
	tokensCmd.Flags().BoolVarP(&TokensOpts.by_acct, "by_acct", "b", false, "consider each address an ERC20 token except the last, whose balance is reported for each token")
	tokensCmd.Flags().BoolVarP(&TokensOpts.no_zero, "no_zero", "n", false, "suppress the display of zero balance accounts")
	tokensCmd.Flags().SortFlags = false
	tokensCmd.PersistentFlags().SortFlags = false

	PostNotes = "[{POSTNOTES}]"
	rootCmd.AddCommand(tokensCmd)
}

func runTokens(cmd *cobra.Command, args []string) {
	options := ""
	if len(TokensOpts.parts) > 0 {
		options += " --parts " + TokensOpts.parts
	}
	if TokensOpts.by_acct {
		options += " --by_acct"
	}
	if TokensOpts.no_zero {
		options += " --no_zero"
	}
	arguments := ""
	for _, arg := range args {
		arguments += " " + arg
	}
	PassItOn("/Users/jrush/.local/bin/chifra/getTokens", options, arguments)
}

func validateTokensArgs(cmd *cobra.Command, args []string) error {
	if len(args) > 0 && args[0] == "12" {
		return ErrFunc(cmd, errors.New("Invalid argument "+args[0]))
	}
	return nil
}
