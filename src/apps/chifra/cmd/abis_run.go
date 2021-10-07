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
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/utils"
	"github.com/spf13/cobra"
)

func validateAbisArgs(cmd *cobra.Command, args []string) error {
	if AbisOpts.classes {
		return makeError("the '{0}' option is not implemented", "--classes")
	}

	if len(AbisOpts.sol) > 0 {
		cleaned := "./" + strings.Replace(AbisOpts.sol, ".sol", "", 1) + ".sol"
		if !utils.FileExists(cleaned) {
			return makeError("file not found at {0}", cleaned)
		}
		return nil
	}

	if len(AbisOpts.find) == 0 && !AbisOpts.known {
		err := validateOneAddr(args)
		if err != nil {
			return err
		}
	}

	err := validateGlobalFlags(cmd, args)
	if err != nil {
		return err
	}
	return nil
}

func runAbis(cmd *cobra.Command, args []string) {
	options := ""
	if AbisOpts.known {
		options += " --known"
	}
	if len(AbisOpts.sol) > 0 {
		options += " --sol " + AbisOpts.sol
	}
	for _, t := range AbisOpts.find {
		options += " --find " + t
	}
	if AbisOpts.source {
		options += " --source"
	}
	if AbisOpts.classes {
		options += " --classes"
	}
	arguments := ""
	for _, arg := range args {
		arguments += " " + arg
	}
	PassItOn("grabABI", options, arguments)
}
