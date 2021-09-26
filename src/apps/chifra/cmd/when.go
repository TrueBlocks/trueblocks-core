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
	"errors"
	"fmt"
	"os"

	"github.com/spf13/cobra"
)

// whenCmd represents the when command
var whenCmd = &cobra.Command{
	Use:   usageWhen,
	Short: shortWhen,
	Long:  longWhen,
	Run:   runWhen,
	Args:  ValidatePositionals(validateWhenArgs, cobra.MinimumNArgs(1)),
}

var usageWhen = `when [flags] < block | date > [ block... | date... ]

Arguments:
  blocks - one or more dates, block numbers, hashes, or special named blocks (see notes) (required)`

var shortWhen = "find block(s) based on date, blockNum, timestamp, or 'special'"

var longWhen = `Purpose:
  Find block(s) based on date, blockNum, timestamp, or 'special'.`

type whenOptionsType struct {
	list       bool
	timestamps bool
	check      bool
	fix        bool
	count      bool
	skip       uint64
}

var WhenOpts whenOptionsType

func init() {
	whenCmd.SetOut(os.Stderr)

	whenCmd.Flags().SortFlags = false
	whenCmd.PersistentFlags().SortFlags = false
	whenCmd.Flags().BoolVarP(&WhenOpts.list, "list", "l", false, "export a list of the 'special' blocks")
	whenCmd.Flags().BoolVarP(&WhenOpts.timestamps, "timestamps", "t", false, "ignore other options and generate timestamps only")
	whenCmd.Flags().BoolVarP(&WhenOpts.check, "check", "c", false, "available only with --timestamps, checks the validity of the timestamp data (hidden)")
	whenCmd.Flags().BoolVarP(&WhenOpts.fix, "fix", "f", false, "available only with --timestamps, fixes incorrect timestamps if any (hidden)")
	whenCmd.Flags().BoolVarP(&WhenOpts.count, "count", "u", false, "available only with --timestamps, returns the number of timestamps in the cache (hidden)")
	whenCmd.Flags().Uint64VarP(&WhenOpts.skip, "skip", "s", 0, "only applicable if --timestamps is on, the step between block numbers in the export (hidden)")
	whenCmd.Flags().SortFlags = false
	whenCmd.PersistentFlags().SortFlags = false

	PostNotes = "[{POSTNOTES}]"
	rootCmd.AddCommand(whenCmd)
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
	if WhenOpts.skip > 0 {
		options += " --skip " + fmt.Sprintf("%d", WhenOpts.skip)
	}
	arguments := ""
	for _, arg := range args {
		arguments += " " + arg
	}
	PassItOn("/Users/jrush/.local/bin/chifra/whenBlock", options, arguments)
}

func validateWhenArgs(cmd *cobra.Command, args []string) error {
	if len(args) > 0 && args[0] == "12" {
		return ErrFunc(cmd, errors.New("Invalid argument "+args[0]))
	}
	return nil
}
