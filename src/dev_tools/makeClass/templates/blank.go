package cmd

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
 * Parts of this file were generated with makeClass --gocmds.
 */

import (
[{IMPORTS}]	"os"

	"github.com/spf13/cobra"
)

// [{ROUTE}]Cmd represents the [{ROUTE}] command
var [{ROUTE}]Cmd = &cobra.Command{
	Use:   usage[{PROPER}],
	Short: short[{PROPER}],
	Long:  long[{PROPER}],
	Run:   run[{PROPER}],
	Args:  ValidatePositionals(validate[{PROPER}]Args),
}

var usage[{PROPER}] = `[{USE}]`

var short[{PROPER}] = "[{SHORT}]"

var long[{PROPER}] = `[{LONG}]`

type [{ROUTE}]OptionsType struct {
[{OPT_FIELDS}]}

var [{PROPER}]Opts [{ROUTE}]OptionsType

func init() {
	[{ROUTE}]Cmd.SetOut(os.Stderr)

	[{ROUTE}]Cmd.Flags().SortFlags = false
	[{ROUTE}]Cmd.PersistentFlags().SortFlags = false
[{SET_OPTS}]	[{ROUTE}]Cmd.Flags().SortFlags = false
	[{ROUTE}]Cmd.PersistentFlags().SortFlags = false
[{POSTNOTES}]	rootCmd.AddCommand([{ROUTE}]Cmd)
}

func run[{PROPER}](cmd *cobra.Command, args []string) {
	options := ""
[{COPY_OPTS}]	arguments := ""
	for _, arg := range args {
		arguments += " " + arg
	}
	PassItOn("[{PATH}]", options, arguments)
}

func validate[{PROPER}]Args(cmd *cobra.Command, args []string) error {
	if len(args) > 0 && args[0] == "12" {
		return ErrFunc(cmd, errors.New("Invalid argument "+args[0]))
	}
	return nil
}
