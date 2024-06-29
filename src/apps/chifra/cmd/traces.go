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
	tracesPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/traces"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// tracesCmd represents the traces command
var tracesCmd = &cobra.Command{
	Use:     usageTraces,
	Long:    longTraces,
	Version: versionText,
	PreRun: outputHelpers.PreRunWithJsonWriter("traces", func() *globals.GlobalOptions {
		return &tracesPkg.GetOptions().Globals
	}),
	RunE: file.RunWithFileSupport("traces", tracesPkg.RunTraces, tracesPkg.ResetOptions),
	PostRun: outputHelpers.PostRunWithJsonWriter(func() *globals.GlobalOptions {
		return &tracesPkg.GetOptions().Globals
	}),
}

const usageTraces = `traces [flags] <tx_id> [tx_id...]

Arguments:
  transactions - a space-separated list of one or more transaction identifiers (required)`

const longTraces = `Purpose:
  Retrieve traces for the given transaction(s).`

const notesTraces = `
Notes:
  - The transactions list may be one or more transaction hashes, blockNumber.transactionID pairs, or a blockHash.transactionID pairs.
  - This tool checks for valid input syntax, but does not check that the transaction requested actually exists.
  - If the queried node does not store historical state, the results for most older transactions are undefined.
  - A bang separated filter has the following fields (at least one of which is required) and is separated with a bang (!): fromBlk, toBlk, fromAddr, toAddr, after, count.
  - This command requires your RPC to provide trace data. See the README for more information.`

func init() {
	var capabilities caps.Capability // capabilities for chifra traces
	capabilities = capabilities.Add(caps.Default)
	capabilities = capabilities.Add(caps.Caching)
	capabilities = capabilities.Add(caps.Ether)
	capabilities = capabilities.Add(caps.Names)

	tracesCmd.Flags().SortFlags = false

	tracesCmd.Flags().BoolVarP(&tracesPkg.GetOptions().Articulate, "articulate", "a", false, `articulate the retrieved data if ABIs can be found`)
	tracesCmd.Flags().StringVarP(&tracesPkg.GetOptions().Filter, "filter", "f", "", `call the node's trace_filter routine with bang-separated filter`)
	tracesCmd.Flags().BoolVarP(&tracesPkg.GetOptions().Count, "count", "U", false, `display only the number of traces for the transaction (fast)`)
	globals.InitGlobals("traces", tracesCmd, &tracesPkg.GetOptions().Globals, capabilities)

	tracesCmd.SetUsageTemplate(UsageWithNotes(notesTraces))
	tracesCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand(tracesCmd)
}
