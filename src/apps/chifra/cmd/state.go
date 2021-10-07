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

// stateCmd represents the state command
var stateCmd = &cobra.Command{
	Use:   usageState,
	Short: shortState,
	Long:  longState,
	Run:   runState,
	Args:  validateStateArgs,
}

var usageState = `state [flags] <address> [address...] [block...]

Arguments:
  addrs - one or more addresses (0x...) from which to retrieve balances (required)
  blocks - an optional list of one or more blocks at which to report balances, defaults to 'latest'`

var shortState = "retrieve account balance(s) for one or more addresses at given block(s)"

var longState = `Purpose:
  Retrieve account balance(s) for one or more addresses at given block(s).`

var notesState = `
Notes:
  - An address must start with '0x' and be forty-two characters long.
  - blocks may be a space-separated list of values, a start-end range, a special, or any combination.
  - If the queried node does not store historical state, the results are undefined.
  - special blocks are detailed under chifra when --list.
  - balance is the default mode. To select a single mode use none first, followed by that mode.
  - You may specify multiple modes on a single line.`

type stateOptionsType struct {
	parts   []string
	changes bool
	no_zero bool
	call    string
}

var StateOpts stateOptionsType

func init() {
	stateCmd.SetOut(os.Stderr)

	stateCmd.Flags().SortFlags = false
	stateCmd.PersistentFlags().SortFlags = false
	stateCmd.Flags().StringSliceVarP(&StateOpts.parts, "parts", "p", nil, `control which state to export
One or more of [ none | some | all | balance | nonce | code | storage | deployed | accttype ]`)
	stateCmd.Flags().BoolVarP(&StateOpts.changes, "changes", "c", false, "only report a balance when it changes from one block to the next")
	stateCmd.Flags().BoolVarP(&StateOpts.no_zero, "no_zero", "n", false, "suppress the display of zero balance accounts")
	stateCmd.Flags().StringVarP(&StateOpts.call, "call", "a", "", "a bang-separated string consisting of address!4-byte!bytes (hidden)")
	if !utils.IsTestMode() {
		stateCmd.Flags().MarkHidden("call")
	}
	stateCmd.Flags().SortFlags = false
	stateCmd.PersistentFlags().SortFlags = false

	stateCmd.SetUsageTemplate(UsageWithNotes(notesState))
	rootCmd.AddCommand(stateCmd)
}

// EXISTING_CODE
// EXISTING_CODE
