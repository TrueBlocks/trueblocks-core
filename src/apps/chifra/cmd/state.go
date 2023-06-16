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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// stateCmd represents the state command
var stateCmd = &cobra.Command{
	Use:     usageState,
	Short:   shortState,
	Long:    longState,
	Version: versionText,
	PreRun: outputHelpers.PreRunWithJsonWriter("state", func() *globals.GlobalOptions {
		return &statePkg.GetOptions().Globals
	}),
	RunE:    file.RunWithFileSupport("state", statePkg.RunState, statePkg.ResetOptions),
	PostRun: outputHelpers.PostRunWithJsonWriter(func() *globals.GlobalOptions {
		return &statePkg.GetOptions().Globals
	}),
}

const usageState = `state [flags] <address> [address...] [block...]

Arguments:
  addrs - one or more addresses (0x...) from which to retrieve balances (required)
  blocks - an optional list of one or more blocks at which to report balances, defaults to 'latest'`

const shortState = "retrieve account balance(s) for one or more addresses at given block(s)"

const longState = `Purpose:
  Retrieve account balance(s) for one or more addresses at given block(s).`

const notesState = `
Notes:
  - An address must be either an ENS name or start with '0x' and be forty-two characters long.
  - Blocks is a space-separated list of values, a start-end range, a special, or any combination.
  - If the queried node does not store historical state, the results are undefined.
  - Special blocks are detailed under chifra when --list.
  - Balance is the default mode. To select a single mode use none first, followed by that mode.
  - You may specify multiple modes on a single line.`

func init() {
	stateCmd.Flags().SortFlags = false

	stateCmd.Flags().StringSliceVarP(&statePkg.GetOptions().Parts, "parts", "p", nil, `control which state to export
One or more of [ none | some | all | balance | nonce | code | proxy | deployed | accttype ]`)
	stateCmd.Flags().BoolVarP(&statePkg.GetOptions().Changes, "changes", "c", false, "only report a balance when it changes from one block to the next")
	stateCmd.Flags().BoolVarP(&statePkg.GetOptions().NoZero, "no_zero", "n", false, "suppress the display of zero balance accounts")
	stateCmd.Flags().StringVarP(&statePkg.GetOptions().Call, "call", "a", "", "Solidity-like function call: balanceOf(0x0C316b7042b419d07d343F2f4F5Bd54FF731183d) or four-byte with arguments: 0x70a08231(0x0C316b7042b419d07d343F2f4F5Bd54FF731183d) or full encoded data parameter to eth_call")
	stateCmd.Flags().StringVarP(&statePkg.GetOptions().ProxyFor, "proxy_for", "r", "", "for the --call option only, redirects calls to this implementation")
	globals.InitGlobals(stateCmd, &statePkg.GetOptions().Globals)

	stateCmd.SetUsageTemplate(UsageWithNotes(notesState))
	stateCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand(stateCmd)
}
