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
	statePkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/state"
	"github.com/spf13/cobra"
)

// stateCmd represents the state command
var stateCmd = &cobra.Command{
	Use:   usageState,
	Short: shortState,
	Long:  longState,
	RunE:  statePkg.RunState,
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

func init() {
	stateCmd.Flags().SortFlags = false

	stateCmd.Flags().StringSliceVarP(&statePkg.Options.Parts, "parts", "p", nil, `control which state to export
One or more of [ none | some | all | balance | nonce | code | storage | deployed | accttype ]`)
	stateCmd.Flags().BoolVarP(&statePkg.Options.Changes, "changes", "c", false, "only report a balance when it changes from one block to the next")
	stateCmd.Flags().BoolVarP(&statePkg.Options.NoZero, "no_zero", "n", false, "suppress the display of zero balance accounts")
	stateCmd.Flags().StringVarP(&statePkg.Options.Call, "call", "a", "", "a bang-separated string consisting of address!4-byte!bytes (hidden)")
	stateCmd.Flags().StringVarP(&statePkg.Options.ProxyFor, "proxy_for", "r", "", "for the --call option only, redirects calls to this implementation (hidden)")
	if os.Getenv("TEST_MODE") != "true" {
		stateCmd.Flags().MarkHidden("call")
		stateCmd.Flags().MarkHidden("proxy_for")
	}
	globals.InitGlobals(stateCmd, &statePkg.Options.Globals)

	stateCmd.SetUsageTemplate(UsageWithNotes(notesState))

	stateCmd.SetOut(os.Stderr)
	if statePkg.Options.Globals.ApiMode {
		stateCmd.SetErr(os.Stdout)
	}

	chifraCmd.AddCommand(stateCmd)
}
