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

func validateScrapeArgs(cmd *cobra.Command, args []string) error {
	err := validateGlobalFlags(cmd, args)
	if err != nil {
		return err
	}
	return nil
}

func runScrape(cmd *cobra.Command, args []string) {
	options := ""
	if ScrapeOpts.pin {
		options += " --pin"
	}
	if ScrapeOpts.sleep != 14 {
		options += " --sleep " + fmt.Sprintf("%.1f", ScrapeOpts.sleep)
	}
	if ScrapeOpts.n_blocks != 2000 {
		options += " --n_blocks " + fmt.Sprintf("%d", ScrapeOpts.n_blocks)
	}
	if ScrapeOpts.n_block_procs != 10 {
		options += " --n_block_procs " + fmt.Sprintf("%d", ScrapeOpts.n_block_procs)
	}
	if ScrapeOpts.n_addr_procs != 20 {
		options += " --n_addr_procs " + fmt.Sprintf("%d", ScrapeOpts.n_addr_procs)
	}
	arguments := ""
	for _, arg := range args {
		arguments += " " + arg
	}
	PassItOn(GetCommandPath("blockScrape"), options, arguments)
}
