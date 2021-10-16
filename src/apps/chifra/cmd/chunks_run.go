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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	"github.com/spf13/cobra"
)

func validateChunksArgs(cmd *cobra.Command, args []string) error {
	list := ChunksOpts.list
	check := ChunksOpts.check
	extract := ChunksOpts.extract
	stats := ChunksOpts.stats

	if !list && !check && len(extract) == 0 {
		return validate.Usage("You must choose at least one of {0}.", "--list, --extract, or --check")
	}

	if (list && check) || (list && len(extract) > 0) {
		return validate.Usage("Please choose just one of {0}.", "--list, --extract, or --check")
	}

	if stats && !list {
		return validate.Usage("The {0} option is only available with the {1} option.", "--stats", "--list")
	}

	err := validate.ValidateEnum("--extract", ChunksOpts.extract, "[header|addr_table|app_table|chunks|blooms]")
	if err != nil {
		return err
	}

	err = validateGlobalFlags(cmd, args)
	if err != nil {
		return err
	}

	return nil
}

func runChunks(cmd *cobra.Command, args []string) {
	options := ""
	if ChunksOpts.list {
		options += " --list"
	}
	if ChunksOpts.check {
		options += " --check"
	}
	if len(ChunksOpts.extract) > 0 {
		options += " --extract " + ChunksOpts.extract
	}
	if ChunksOpts.stats {
		options += " --stats"
	}
	arguments := ""
	for _, arg := range args {
		arguments += " " + arg
	}
	PassItOn("chunkMan", options, arguments)
}
