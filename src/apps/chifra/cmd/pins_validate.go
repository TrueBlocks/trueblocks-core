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

func validatePinsArgs(cmd *cobra.Command, args []string) error {
	if !PinsOpts.list && !PinsOpts.init && !PinsOpts.freshen {
		return makeError("You must choose at least one of {0}, {1}, or {2}", "--list", "--init", "--freshen")
	}

	if PinsOpts.init_all {
		return makeError("Flag --init_all has been deprecated, use --init --all instead")
	}

	err := validateGlobalFlags(cmd, args)
	if err != nil {
		return err
	}

	return nil
}
