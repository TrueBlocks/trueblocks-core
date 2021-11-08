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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/cmd/abis"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	"github.com/spf13/cobra"
)

func validateAbisArgs(cmd *cobra.Command, args []string) error {
	if AbisOpts.classes {
		return validate.Usage("the '{0}' option is not implemented", "--classes")
	}

	if len(AbisOpts.find) == 0 && !AbisOpts.known {
		err := validate.ValidateAtLeastOneAddr(args)
		if err != nil {
			return err
		}
	}

	if AbisOpts.sol && len(AbisOpts.find) > 0 {
		return validate.Usage("Please choose only one of --sol or --find.")
	}

	if AbisOpts.sol {
		for _, sol := range args {
			if sol == "" {
				continue
			}
			cleaned := "./" + strings.Replace(sol, ".sol", "", 1) + ".sol"
			if !utils.FileExists(cleaned) {
				return validate.Usage("Solidity file not found at {0}", cleaned)
			}
		}
	} else {
		for _, term := range AbisOpts.find {
			ok1, err1 := validate.IsValidFourByte(term)
			if !ok1 && len(term) < 10 {
				return err1
			}
			ok2, err2 := validate.IsValidTopic(term)
			if !ok2 && len(term) > 66 {
				return err2
			}
			if !ok1 && !ok2 {
				if len(term) > 43 {
					// more than halfway reports topic
					return err2
				}
				return err1
			} else {
				validate.Errors = nil
			}
		}

	}

	err := validateGlobalFlags(cmd, args)
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

	if len(AbisOpts.find) > 0 {
		// These have already been validated
		abis.HandleFind(AbisOpts.find)
		return
	}

	options := ""
	if AbisOpts.known {
		options += " --known"
	}
	if AbisOpts.sol {
		options += " --sol"
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
