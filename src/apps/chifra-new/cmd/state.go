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

type stateOptionsType struct {
	parts   string
	changes bool
	no_zero bool
	call    string
}

var StateOpts stateOptionsType

// stateCmd represents the state command
var stateCmd = &cobra.Command{
	Use: `state [flags] <address> [address...] [block...]

Arguments:
  addrs - one or more addresses (0x...) from which to retrieve balances (required)
  blocks - an optional list of one or more blocks at which to report balances, defaults to 'latest'`,
	Short: "retrieve account balance(s) for one or more addresses at given block(s)",
	Long: `Purpose:
  Retrieve account balance(s) for one or more addresses at given block(s).`,
	Run: runState,
}

func init() {
	stateCmd.Flags().SortFlags = false
	stateCmd.PersistentFlags().SortFlags = false
	stateCmd.SetOut(os.Stderr)

	stateCmd.Flags().StringVarP(&StateOpts.parts, "parts", "p", "", "control which state to export")
	stateCmd.Flags().BoolVarP(&StateOpts.changes, "changes", "c", false, "only report a balance when it changes from one block to the next")
	stateCmd.Flags().BoolVarP(&StateOpts.no_zero, "no_zero", "n", false, "suppress the display of zero balance accounts")
	stateCmd.Flags().StringVarP(&StateOpts.call, "call", "a", "", "a bang-separated string consisting of address!4-byte!bytes")

	rootCmd.AddCommand(stateCmd)
}

func runState(cmd *cobra.Command, args []string) {
	options := ""
	if len(StateOpts.parts) > 0 {
		options += " --parts " + StateOpts.parts
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
	for _, arg := range args {
		options += " " + arg
	}
	PassItOn("/Users/jrush/.local/bin/chifra/getState", options, strconv.FormatUint(0, 10))
}
