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
	"fmt"

	"github.com/spf13/cobra"
)

func validateBlocksArgs(cmd *cobra.Command, args []string) error {
	err := validateGlobalFlags(cmd, args)
	if err != nil {
		return err
	}

	return nil
}

func runBlocks(cmd *cobra.Command, args []string) {
	options := ""
	if BlocksOpts.hashes {
		options += " --hashes"
	}
	if BlocksOpts.uncles {
		options += " --uncles"
	}
	if BlocksOpts.trace {
		options += " --trace"
	}
	if BlocksOpts.apps {
		options += " --apps"
	}
	if BlocksOpts.uniq {
		options += " --uniq"
	}
	if BlocksOpts.uniq_tx {
		options += " --uniq_tx"
	}
	if BlocksOpts.count {
		options += " --count"
	}
	if BlocksOpts.cache {
		options += " --cache"
	}
	if BlocksOpts.list > 0 {
		options += " --list " + fmt.Sprintf("%d", BlocksOpts.list)
	}
	if BlocksOpts.list_count != 20 {
		options += " --list_count " + fmt.Sprintf("%d", BlocksOpts.list_count)
	}
	arguments := ""
	for _, arg := range args {
		arguments += " " + arg
	}
	PassItOn("getBlocks", options, arguments)
}
