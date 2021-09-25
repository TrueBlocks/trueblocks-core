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

import (
	"os"
	"strconv"

	"github.com/spf13/cobra"
)

type tokensOptionsType struct {
	parts   string
	by_acct bool
	no_zero bool
}

var TokensOpts tokensOptionsType

// tokensCmd represents the tokens command
var tokensCmd = &cobra.Command{
	Use: `tokens [flags] <address> <address> [address...] [block...]

Arguments:
  addrs - two or more addresses (0x...), the first is an ERC20 token, balances for the rest are reported (required)
  blocks - an optional list of one or more blocks at which to report balances, defaults to 'latest'`,
	Short: "retrieve token balance(s) for one or more addresses at given block(s)",
	Long: `Purpose:
  Retrieve token balance(s) for one or more addresses at given block(s).`,
	Run: runTokens,
}

func init() {
	tokensCmd.Flags().SortFlags = false
	tokensCmd.PersistentFlags().SortFlags = false
	tokensCmd.SetOut(os.Stderr)

	tokensCmd.Flags().StringVarP(&TokensOpts.parts, "parts", "p", "", "one or more parts of the token information to retreive")
	tokensCmd.Flags().BoolVarP(&TokensOpts.by_acct, "by_acct", "b", false, "consider each address an ERC20 token except the last, whose balance is reported for each token")
	tokensCmd.Flags().BoolVarP(&TokensOpts.no_zero, "no_zero", "n", false, "suppress the display of zero balance accounts")

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
	for _, arg := range args {
		options += " " + arg
	}
	PassItOn("/Users/jrush/.local/bin/chifra/getTokens", options, strconv.FormatUint(0, 10))
}
