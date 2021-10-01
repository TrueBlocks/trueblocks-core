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
package cmd

import (
	"github.com/spf13/cobra"
)

func validateTokensArgs(cmd *cobra.Command, args []string) error {
	// special case for tokens which don't allow --dollars display
	if RootOpts.dollars {
		return makeError("The --dollars option does not work with chifra token.")
	}
	err := validateGlobalFlags(cmd, args)
	if err != nil {
		return err
	}
	return nil
}

func runTokens(cmd *cobra.Command, args []string) {
	options := ""
	for _, t := range TokensOpts.parts {
		options += " --parts " + t
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
	PassItOn(GetCommandPath("getTokens"), options, arguments)
}
