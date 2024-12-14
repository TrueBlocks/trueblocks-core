// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package cmd

// EXISTING_CODE
import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	statePkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/state"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// stateCmd represents the state command
var stateCmd = &cobra.Command{
	Use:     usageState,
	Long:    longState,
	Version: versionText,
	PreRun: outputHelpers.PreRunWithJsonWriter("state", func() *globals.GlobalOptions {
		return &statePkg.GetOptions().Globals
	}),
	RunE: file.RunWithFileSupport("state", statePkg.RunState, statePkg.ResetOptions),
	PostRun: outputHelpers.PostRunWithJsonWriter(func() *globals.GlobalOptions {
		return &statePkg.GetOptions().Globals
	}),
}

const usageState = `state [flags] <address> [address...] [block...]

Arguments:
  addrs - one or more addresses (0x...) from which to retrieve balances (required)
  blocks - an optional list of one or more blocks at which to report balances, defaults to 'latest'`

const longState = `Purpose:
  Retrieve account balance(s) for one or more addresses at given block(s).`

const notesState = `
Notes:
  - An address must be either an ENS name or start with '0x' and be forty-two characters long.
  - Blocks is a space-separated list of values, a start-end range, a special, or any combination.
  - If the queried node does not store historical state, the results are undefined.
  - Special blocks are detailed under chifra when --list.
  - Balance is the default mode. To select a single mode use none first, followed by that mode.
  - Valid parameters for --calldata include Solidity-like syntax: balanceOf(0x316b...), a four-byte followed by parameters: 0x70a08231(0x316b...), or encoded input data.
  - You may specify multiple parts on a single line.
  - In the --call string, you may separate multiple calls with a colon.
  - Your use of the unaudited --send option legally absolves, in any jurisdiction, TrueBlocks, LLC or any associated parties from liability or loss related to such use.
  - The --send option does not validate its input before sending your transaction to the network. If you provide invalid data, you may lose your funds. Be warned.
  - As of version 4.0.0, use --call --calldata <cmd> to provide your command.
  - --calldata may be one or more solidity calls, four-byte plus parameters, or encoded call data strings.`

func init() {
	var capabilities caps.Capability // capabilities for chifra state
	capabilities = capabilities.Add(caps.Default)
	capabilities = capabilities.Add(caps.Caching)
	capabilities = capabilities.Add(caps.Ether)
	capabilities = capabilities.Add(caps.Names)

	stateCmd.Flags().SortFlags = false

	stateCmd.Flags().StringSliceVarP(&statePkg.GetOptions().Parts, "parts", "p", nil, `control which state to export
One or more of [ balance | nonce | code | proxy | deployed | accttype | some | all ]`)
	stateCmd.Flags().BoolVarP(&statePkg.GetOptions().Changes, "changes", "c", false, `only report a balance when it changes from one block to the next`)
	stateCmd.Flags().BoolVarP(&statePkg.GetOptions().NoZero, "no_zero", "z", false, `suppress the display of zero balance accounts`)
	stateCmd.Flags().BoolVarP(&statePkg.GetOptions().Send, "send", "s", false, `writes a transaction to an address (see docs for more information) (hidden)`)
	stateCmd.Flags().StringVarP(&statePkg.GetOptions().Calldata, "calldata", "d", "", `for commands (--call or --send), provides the call data (in various forms) for the command (may be empty for --send)`)
	stateCmd.Flags().BoolVarP(&statePkg.GetOptions().Articulate, "articulate", "a", false, `for commands only, articulate the retrieved data if ABIs can be found`)
	stateCmd.Flags().StringVarP(&statePkg.GetOptions().ProxyFor, "proxy_for", "r", "", `for commands only, redirects calls to this implementation`)
	if os.Getenv("TEST_MODE") != "true" {
		_ = stateCmd.Flags().MarkHidden("send")
	}
	globals.InitGlobals("state", stateCmd, &statePkg.GetOptions().Globals, capabilities)

	stateCmd.SetUsageTemplate(UsageWithNotes(notesState))
	stateCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand(stateCmd)
}
