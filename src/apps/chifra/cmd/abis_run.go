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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/cmd/abis"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	"github.com/spf13/cobra"
)

func validateAbisArgs(cmd *cobra.Command, args []string) error {
	err := abis.ValidateOptions(&AbisOpts, args)
	if err != nil {
		return err
	}
	err = validateGlobalFlags(cmd, args)
	if err != nil {
		return err
	}

	return nil
}

func runAbis(cmd *cobra.Command, args []string) {
	// This only happens in API mode when there's been an error. Here, we print the error
	if len(validate.Errors) > 0 {
		output.PrintJson(&output.JsonFormatted{})
		return
	}

	if len(AbisOpts.Find) > 0 {
		// These have already been validated
		abis.HandleFind(AbisOpts.Find)
		return
	}

	options := ""
	if AbisOpts.Known {
		options += " --known"
	}
	if AbisOpts.Sol {
		options += " --sol"
	}
	for _, t := range AbisOpts.Find {
		options += " --find " + t
	}
	if AbisOpts.Source {
		options += " --source"
	}
	if AbisOpts.Classes {
		options += " --classes"
	}
	arguments := ""
	for _, arg := range args {
		arguments += " " + arg
	}
	PassItOn("grabABI", options, arguments)
}
