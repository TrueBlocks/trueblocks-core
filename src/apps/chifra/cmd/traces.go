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
	tracesPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/traces"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// tracesCmd represents the traces command
var tracesCmd = &cobra.Command{
	Use:     usageTraces,
	Short:   shortTraces,
	Long:    longTraces,
	Version: versionText,
	RunE:    tracesPkg.RunTraces,
}

var usageTraces = `traces [flags] <tx_id> [tx_id...]

Arguments:
  transactions - a space-separated list of one or more transaction identifiers (required)`

var shortTraces = "retrieve traces for the given transaction(s)"

var longTraces = `Purpose:
  Retrieve traces for the given transaction(s).`

var notesTraces = `
Notes:
  - The transactions list may be one or more transaction hashes, blockNumber.transactionID pairs, or a blockHash.transactionID pairs.
  - This tool checks for valid input syntax, but does not check that the transaction requested actually exists.
  - If the queried node does not store historical state, the results for most older transactions are undefined.
  - A bang separated filter has the following fields (at least one of which is required) and is separated with a bang (!): fromBlk, toBlk, fromAddr, toAddr, after, count.
  - A state diff trace describes, for each modified address, what changed during that trace.`

func init() {
	tracesCmd.Flags().SortFlags = false

	tracesCmd.Flags().BoolVarP(&tracesPkg.Options.Articulate, "articulate", "a", false, "articulate the retrieved data if ABIs can be found")
	tracesCmd.Flags().StringVarP(&tracesPkg.Options.Filter, "filter", "f", "", "call the node's trace_filter routine with bang-separated filter")
	tracesCmd.Flags().BoolVarP(&tracesPkg.Options.Statediff, "statediff", "d", false, "export state diff traces (not implemented)")
	tracesCmd.Flags().BoolVarP(&tracesPkg.Options.Count, "count", "c", false, "show the number of traces for the transaction only (fast)")
	tracesCmd.Flags().BoolVarP(&tracesPkg.Options.SkipDdos, "skip_ddos", "s", false, "skip over the 2016 ddos during export ('on' by default) (hidden)")
	tracesCmd.Flags().Uint64VarP(&tracesPkg.Options.Max, "max", "m", 250, "if --skip_ddos is on, this many traces defines what a ddos transaction is (hidden)")
	if os.Getenv("TEST_MODE") != "true" {
		tracesCmd.Flags().MarkHidden("skip_ddos")
		tracesCmd.Flags().MarkHidden("max")
	}
	globals.InitGlobals(tracesCmd, &tracesPkg.Options.Globals)

	tracesCmd.SetUsageTemplate(UsageWithNotes(notesTraces))
	tracesCmd.SetOut(os.Stderr)

	chifraCmd.AddCommand(tracesCmd)
}
