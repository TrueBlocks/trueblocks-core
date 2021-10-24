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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	"github.com/spf13/cobra"
)

func validateExploreArgs(cmd *cobra.Command, args []string) error {
	TestLogExplore(args)

	if ExploreOpts.google && ExploreOpts.local {
		return validate.Usage("Choose either --google or --local, not both.")
	}

	for _, arg := range args {
		isAddr, _ := validate.IsValidAddress(arg)
		utils.TestLogBool("is_addr", isAddr)

		if ExploreOpts.google {
			if !isAddr {
				return validate.Usage("Option --google allows only an address term.")
			}

		} else {
			isBlock, _ := validate.IsValidBlockId([]string{arg}, validate.ValidBlockId)
			isTx, _ := validate.IsValidTransId([]string{arg}, validate.ValidTransId)
			isFourByte, _ := validate.IsValidFourByte(arg)
			utils.TestLogBool("is_block", isBlock)
			utils.TestLogBool("is__tx", isTx)
			utils.TestLogBool("is_fourbyte", isFourByte)
			if !isAddr && !isBlock && !isFourByte && !isTx {
				return validate.Usage("The argument ({0}) does not appear to be valid.", arg)
			}
		}
	}

	err := validateGlobalFlags(cmd, args)
	if err != nil {
		return err
	}

	return nil
}

func runExplore(cmd *cobra.Command, args []string) {
	options := ""
	if ExploreOpts.local {
		options += " --local"
	}
	if ExploreOpts.google {
		options += " --google"
	}
	arguments := ""
	for _, arg := range args {
		arguments += " " + arg
	}
	PassItOn("fireStorm", options, arguments)
}
