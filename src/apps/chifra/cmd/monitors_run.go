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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/utils"
	"github.com/spf13/cobra"
)

func validateMonitorsArgs(cmd *cobra.Command, args []string) error {
	if !utils.IsApiMode() && !MonitorsOpts.clean {
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

func runMonitors(cmd *cobra.Command, args []string) {
	options := ""
	if MonitorsOpts.appearances {
		options += " --appearances"
	}
	if MonitorsOpts.count {
		options += " --count"
	}
	if MonitorsOpts.clean {
		options += " --clean"
	}
	if MonitorsOpts.freshen {
		options += " --freshen"
	}
	if MonitorsOpts.first_block > 0 {
		options += " --first_block " + fmt.Sprintf("%d", MonitorsOpts.first_block)
	}
	if MonitorsOpts.last_block > 0 {
		options += " --last_block " + fmt.Sprintf("%d", MonitorsOpts.last_block)
	}
	arguments := ""
	for _, arg := range args {
		arguments += " " + arg
	}
	PassItOn("acctExport", options, arguments)
}
