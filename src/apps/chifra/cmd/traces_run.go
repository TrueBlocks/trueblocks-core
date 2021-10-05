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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	"github.com/spf13/cobra"
)

func validateTracesArgs(cmd *cobra.Command, args []string) error {
	err := validate.ValidateGlobalFlags(RootOpts.file, RootOpts.fmt, cmd, args)
	if err != nil {
		return err
	}
	return nil
}

func runTraces(cmd *cobra.Command, args []string) {
	options := ""
	if TracesOpts.articulate {
		options += " --articulate"
	}
	if len(TracesOpts.filter) > 0 {
		options += " --filter " + TracesOpts.filter
	}
	if TracesOpts.statediff {
		options += " --statediff"
	}
	if TracesOpts.count {
		options += " --count"
	}
	if TracesOpts.skip_ddos {
		options += " --skip_ddos"
	}
	if TracesOpts.max != 250 {
		options += " --max " + fmt.Sprintf("%d", TracesOpts.max)
	}
	arguments := ""
	for _, arg := range args {
		arguments += " " + arg
	}
	PassItOn("getTraces", options, arguments)
}
