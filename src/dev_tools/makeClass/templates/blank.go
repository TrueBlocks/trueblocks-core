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
/*
 * The file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */
package cmd

import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/cmd/globals"
[{IMPORTS}]	"github.com/spf13/cobra"
)

// [{ROUTE}]Cmd represents the [{ROUTE}] command
var [{ROUTE}]Cmd = &cobra.Command{
	Use:   usage[{PROPER}],
	Short: short[{PROPER}],
	Long:  long[{PROPER}],
	RunE:  run[{PROPER}],
	Args:  validate[{PROPER}]Args,
[{PERPRERUN}]}

var usage[{PROPER}] = `[{USE}]`

var short[{PROPER}] = "[{SHORT}]"

var long[{PROPER}] = `[{LONG}]`

var notes[{PROPER}] = `[{POSTNOTES}]`

[{OPT_DEF}]func init() {
	[{ROUTE}]Cmd.Flags().SortFlags = false
[{SET_OPTS}][{HIDDEN}]	[{ROUTE}]Cmd.Flags().SortFlags = false
	globals.GlobalOptions([{ROUTE}]Cmd, &[{ROUTE}]Pkg.Options.Globals)

	[{ROUTE}]Cmd.SetUsageTemplate(UsageWithNotes(notes[{PROPER}]))
	[{ROUTE}]Cmd.SetOut(os.Stderr)
	if [{ROUTE}]Pkg.Options.Globals.ApiMode {
		[{ROUTE}]Cmd.SetErr(os.Stdout)
	}
	chifraCmd.AddCommand([{ROUTE}]Cmd)
}
