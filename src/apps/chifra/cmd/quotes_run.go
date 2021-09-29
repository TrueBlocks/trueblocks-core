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

func runQuotes(cmd *cobra.Command, args []string) {
	options := ""
	if QuotesOpts.freshen {
		options += " --freshen"
	}
	if len(QuotesOpts.period) > 0 {
		options += " --period " + QuotesOpts.period
	}
	if len(QuotesOpts.pair) > 0 {
		options += " --pair " + QuotesOpts.pair
	}
	if len(QuotesOpts.feed) > 0 {
		options += " --feed " + QuotesOpts.feed
	}
	arguments := ""
	for _, arg := range args {
		arguments += " " + arg
	}
	PassItOn(GetCommandPath("getQuotes"), options, arguments)
}
