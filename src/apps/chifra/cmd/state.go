// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package cmd

// EXISTING_CODE
import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	statePkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/state"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// stateCmd represents the state command
var stateCmd = &cobra.Command{
	Use:     usageState,
	Short:   shortState,
	Long:    longState,
	Version: versionText,
	RunE:    statePkg.RunState,
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

	stateCmd.Flags().StringSliceVarP(&statePkg.GetOptions().Parts, "parts", "p", nil, `control which state to export
One or more of [ none | some | all | balance | nonce | code | storage | deployed | accttype ]`)
	stateCmd.Flags().BoolVarP(&statePkg.GetOptions().Changes, "changes", "c", false, "only report a balance when it changes from one block to the next")
	stateCmd.Flags().BoolVarP(&statePkg.GetOptions().NoZero, "no_zero", "n", false, "suppress the display of zero balance accounts")
	stateCmd.Flags().StringVarP(&statePkg.GetOptions().Call, "call", "a", "", "a bang-separated string consisting of address!4-byte!bytes (hidden)")
	stateCmd.Flags().StringVarP(&statePkg.GetOptions().ProxyFor, "proxy_for", "r", "", "for the --call option only, redirects calls to this implementation (hidden)")
	if os.Getenv("TEST_MODE") != "true" {
		stateCmd.Flags().MarkHidden("call")
		stateCmd.Flags().MarkHidden("proxy_for")
	}
	globals.InitGlobals(stateCmd, &statePkg.GetOptions().Globals)

	stateCmd.SetUsageTemplate(UsageWithNotes(notesState))
	stateCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand(stateCmd)
}
