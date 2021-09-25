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

import (
	"os"
	"strconv"

	"github.com/spf13/cobra"
)

type [{ROUTE}]OptionsType struct {
[{OPT_FIELDS}]}

var [{PROPER}]Opts [{ROUTE}]OptionsType

// [{ROUTE}]Cmd represents the [{ROUTE}] command
var [{ROUTE}]Cmd = &cobra.Command{
	Use: `[{USE}]`,
	Short: "[{SHORT}]",
	Long: `[{LONG}]`,
	Run: run[{PROPER}],
}

func init() {
	[{ROUTE}]Cmd.Flags().SortFlags = false
	[{ROUTE}]Cmd.PersistentFlags().SortFlags = false
	[{ROUTE}]Cmd.SetOut(os.Stderr)

[{SET_OPTS}]
	rootCmd.AddCommand([{ROUTE}]Cmd)
}

func run[{PROPER}](cmd *cobra.Command, args []string) {
	options := ""
[{COPY_OPTS}]	for _, arg := range args {
		options += " " + arg
	}
	PassItOn("[{PATH}]", options, strconv.FormatUint(0, 10))
}
