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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/cmd/root"
	whenPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/when"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/spf13/cobra"
)

// whenCmd represents the when command
var whenCmd = &cobra.Command{
	Use:   usageWhen,
	Short: shortWhen,
	Long:  longWhen,
	RunE:  whenPkg.Run,
	Args:  whenPkg.Validate,
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

func init() {
	whenCmd.Flags().SortFlags = false
	whenCmd.Flags().BoolVarP(&whenPkg.Options.List, "list", "l", false, "export a list of the 'special' blocks")
	whenCmd.Flags().BoolVarP(&whenPkg.Options.Timestamps, "timestamps", "t", false, "ignore other options and generate timestamps only")
	whenCmd.Flags().BoolVarP(&whenPkg.Options.Check, "check", "c", false, "available only with --timestamps, checks the validity of the timestamp data (hidden)")
	whenCmd.Flags().BoolVarP(&whenPkg.Options.Fix, "fix", "f", false, "available only with --timestamps, fixes incorrect timestamps if any (hidden)")
	whenCmd.Flags().BoolVarP(&whenPkg.Options.Count, "count", "u", false, "available only with --timestamps, returns the number of timestamps in the cache (hidden)")
	if os.Getenv("TEST_MODE") != "true" {
		whenCmd.Flags().MarkHidden("check")
		whenCmd.Flags().MarkHidden("fix")
		whenCmd.Flags().MarkHidden("count")
	}
	whenCmd.Flags().SortFlags = false
	root.GlobalOptions(whenCmd, &whenPkg.Options.Globals)

	whenCmd.SetUsageTemplate(UsageWithNotes(notesWhen))
	whenCmd.SetOut(os.Stderr)
	if utils.IsApiMode() {
		whenCmd.SetErr(os.Stdout)
	}
	rootCmd.AddCommand(whenCmd)
}
