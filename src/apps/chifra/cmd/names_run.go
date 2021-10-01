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

func validateNamesArgs(cmd *cobra.Command, args []string) error {
	err := validateGlobalFlags(cmd, args)
	if err != nil {
		return err
	}

	return nil
}

func runNames(cmd *cobra.Command, args []string) {
	options := ""
	if NamesOpts.expand {
		options += " --expand"
	}
	if NamesOpts.match_case {
		options += " --match_case"
	}
	if NamesOpts.all {
		options += " --all"
	}
	if NamesOpts.custom {
		options += " --custom"
	}
	if NamesOpts.prefund {
		options += " --prefund"
	}
	if NamesOpts.named {
		options += " --named"
	}
	if NamesOpts.addr {
		options += " --addr"
	}
	if NamesOpts.collections {
		options += " --collections"
	}
	if NamesOpts.tags {
		options += " --tags"
	}
	if NamesOpts.to_custom {
		options += " --to_custom"
	}
	if NamesOpts.clean {
		options += " --clean"
	}
	if len(NamesOpts.autoname) > 0 {
		options += " --autoname " + NamesOpts.autoname
	}
	arguments := ""
	for _, arg := range args {
		arguments += " " + arg
	}
	PassItOn(GetCommandPath("ethNames"), options, arguments)
}
