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
package cmd

import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/utils"
	"github.com/spf13/cobra"
)

// whenCmd represents the when command
var whenCmd = &cobra.Command{
	Use:   usageWhen,
	Short: shortWhen,
	Long:  longWhen,
	Run:   runWhen,
	Args:  validateWhenArgs,
}

var usageWhen = `when [flags] < block | date > [ block... | date... ]

Arguments:
  blocks - one or more dates, block numbers, hashes, or special named blocks (see notes) (required)`

var shortWhen = "find block(s) based on date, blockNum, timestamp, or 'special'"

var longWhen = `Purpose:
  Find block(s) based on date, blockNum, timestamp, or 'special'.`

var notesWhen = `
Notes:
  - The block list may contain any combination of number, hash, date, special named blocks.
  - Dates must be formatted in JSON format: YYYY-MM-DD[THH[:MM[:SS]]].`

type whenOptionsType struct {
	list       bool
	timestamps bool
	check      bool
	fix        bool
	count      bool
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
	if !utils.IsTestMode() {
		whenCmd.Flags().MarkHidden("check")
		whenCmd.Flags().MarkHidden("fix")
		whenCmd.Flags().MarkHidden("count")
	}
	whenCmd.Flags().SortFlags = false
	whenCmd.PersistentFlags().SortFlags = false

	whenCmd.SetUsageTemplate(UsageWithNotes(notesWhen))
	rootCmd.AddCommand(whenCmd)
}

func TestLogWhen(args []string) {
	if !utils.IsTestMode() {
		return
	}
}

// EXISTING_CODE
// EXISTING_CODE
