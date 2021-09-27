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
	"os"

	"github.com/spf13/cobra"
)

// stateCmd represents the state command
var stateCmd = &cobra.Command{
	Use:   usageState,
	Short: shortState,
	Long:  longState,
	Run:   runState,
	Args:  ValidatePositionals(validateStateArgs),
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
One or more of none, some, all, balance, nonce, code, storage, deployed, accttype`)
	stateCmd.Flags().BoolVarP(&StateOpts.changes, "changes", "c", false, "only report a balance when it changes from one block to the next")
	stateCmd.Flags().BoolVarP(&StateOpts.no_zero, "no_zero", "n", false, "suppress the display of zero balance accounts")
	stateCmd.Flags().StringVarP(&StateOpts.call, "call", "a", "", "a bang-separated string consisting of address!4-byte!bytes (hidden)")
	if IsTestMode() == false {
		stateCmd.Flags().MarkHidden("call")
	}
	stateCmd.Flags().SortFlags = false
	stateCmd.PersistentFlags().SortFlags = false

	stateCmd.SetUsageTemplate(HelpWithNotes(notesState))
	rootCmd.AddCommand(stateCmd)
}

func runState(cmd *cobra.Command, args []string) {
	options := ""
	for _, t := range StateOpts.parts {
		options += " --parts " + t
	}
	if StateOpts.changes {
		options += " --changes"
	}
	if StateOpts.no_zero {
		options += " --no_zero"
	}
	if len(StateOpts.call) > 0 {
		options += " --call " + StateOpts.call
	}
	arguments := ""
	for _, arg := range args {
		arguments += " " + arg
	}
	PassItOn(GetCommandPath("getState"), options, arguments)
}

func validateStateArgs(cmd *cobra.Command, args []string) error {
	if len(args) > 0 && args[0] == "12" {
		return ErrFunc(cmd, errors.New("Invalid argument "+args[0]))
	}
	return nil
}
