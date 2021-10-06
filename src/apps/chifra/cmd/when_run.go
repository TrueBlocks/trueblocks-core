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
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	"github.com/spf13/cobra"
)

func validateWhenArgs(cmd *cobra.Command, args []string) error {
	// if !WhenOpts.list {
	// 	if len(args) == 0 {
	// 		return errors.New(fmtError("You must provide either a date or a block number"))
	// 	}
	// }
	err := validateGlobalFlags(cmd, args)
	if err != nil {
		return err
	}

	validationErr := validate.ValidateBlockIdentifiers(args, validate.ValidArgumentAll, 1)

	if validationErr != nil {
		if invalidLiteral, ok := validationErr.(*validate.InvalidIdentifierLiteralError); ok {
			return invalidLiteral
		}

		if errors.Is(validationErr, validate.ErrTooManyRanges) {
			return errors.New("Specify only a single block range at a time.")
		}

		return validationErr
	}

	return nil
}

func runWhen(cmd *cobra.Command, args []string) {
	options := ""
	if WhenOpts.list {
		options += " --list"
	}
	if WhenOpts.timestamps {
		options += " --timestamps"
	}
	if WhenOpts.check {
		options += " --check"
	}
	if WhenOpts.fix {
		options += " --fix"
	}
	if WhenOpts.count {
		options += " --count"
	}
	arguments := ""
	for _, arg := range args {
		arguments += " " + arg
	}
	PassItOn("whenBlock", options, arguments)
}
