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
 * Parts of this file were generated with makeClass --gocmds. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

import (
	// EXISTING_CODE
	// EXISTING_CODE
)

// [{ROUTE}]Cmd represents the [{ROUTE}] command
var [{ROUTE}]Cmd = &cobra.Command{
	Use:   usage[{PROPER}],
	Short: short[{PROPER}],
	Long:  long[{PROPER}],
	Run:   run[{PROPER}],
	Args:  validate[{PROPER}]Args,
[{PERPRERUN}]}

var usage[{PROPER}] = `[{USE}]`

var short[{PROPER}] = "[{SHORT}]"

var long[{PROPER}] = `[{LONG}]`

var notes[{PROPER}] = `[{POSTNOTES}]`

type [{ROUTE}]OptionsType struct {
[{OPT_FIELDS}]}

var [{PROPER}]Opts [{ROUTE}]OptionsType

func init() {
	[{ROUTE}]Cmd.SetOut(os.Stderr)

	[{ROUTE}]Cmd.Flags().SortFlags = false
	[{ROUTE}]Cmd.PersistentFlags().SortFlags = false
[{SET_OPTS}][{HIDDEN}]	[{ROUTE}]Cmd.Flags().SortFlags = false
	[{ROUTE}]Cmd.PersistentFlags().SortFlags = false

	[{ROUTE}]Cmd.SetUsageTemplate(HelpWithNotes(notes[{PROPER}]))
	rootCmd.AddCommand([{ROUTE}]Cmd)
}

func run[{PROPER}](cmd *cobra.Command, args []string) {
	options := ""
[{COPY_OPTS}]	arguments := ""
	for _, arg := range args {
		arguments += " " + arg
	}
	// EXISTING_CODE
	// EXISTING_CODE
	PassItOn(GetCommandPath("[{PATH}]"), options, arguments)
}

// EXISTING_CODE
// EXISTING_CODE

func validate[{PROPER}]Args(cmd *cobra.Command, args []string) error {
	var err error
	// EXISTING_CODE
	// EXISTING_CODE
	err = validateGlobalFlags(cmd, args)
	if err != nil {
		return err
	}
	return nil
}
