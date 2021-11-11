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

	tracesPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/cmd/traces"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/spf13/cobra"
)

// tracesCmd represents the traces command
var tracesCmd = &cobra.Command{
	Use:   usageTraces,
	Short: shortTraces,
	Long:  longTraces,
	Run:   tracesPkg.Run,
	Args:  tracesPkg.Validate,
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
	tracesCmd.SetOut(os.Stderr)

	tracesCmd.Flags().SortFlags = false
	tracesCmd.PersistentFlags().SortFlags = false
	tracesCmd.Flags().BoolVarP(&tracesPkg.Options.Articulate, "articulate", "a", false, "articulate the retrieved data if ABIs can be found")
	tracesCmd.Flags().StringVarP(&tracesPkg.Options.Filter, "filter", "f", "", "call the node's trace_filter routine with bang-separated filter")
	tracesCmd.Flags().BoolVarP(&tracesPkg.Options.Statediff, "statediff", "d", false, "export state diff traces (not implemented)")
	tracesCmd.Flags().BoolVarP(&tracesPkg.Options.Count, "count", "c", false, "show the number of traces for the transaction only (fast)")
	tracesCmd.Flags().BoolVarP(&tracesPkg.Options.Skip_Ddos, "skip_ddos", "s", false, "skip over the 2016 ddos during export ('on' by default) (hidden)")
	tracesCmd.Flags().Uint64VarP(&tracesPkg.Options.Max, "max", "m", 250, "if --skip_ddos is on, this many traces defines what a ddos transaction is (hidden)")
	if !utils.IsTestMode() {
		tracesCmd.Flags().MarkHidden("skip_ddos")
		tracesCmd.Flags().MarkHidden("max")
	}
	tracesCmd.Flags().SortFlags = false
	tracesCmd.PersistentFlags().SortFlags = false

	tracesCmd.SetUsageTemplate(UsageWithNotes(notesTraces))
	rootCmd.AddCommand(tracesCmd)
}
