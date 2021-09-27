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
	"fmt"
	"os"

	"github.com/spf13/cobra"
)

// tracesCmd represents the traces command
var tracesCmd = &cobra.Command{
	Use:   usageTraces,
	Short: shortTraces,
	Long:  longTraces,
	Run:   runTraces,
	Args:  ValidatePositionals(validateTracesArgs),
}

var usageTraces = `traces [flags] <tx_id> [tx_id...]

Arguments:
  transactions - a space-separated list of one or more transaction identifiers (required)`

var shortTraces = "retrieve traces for the given transaction(s)"

var longTraces = `Purpose:
  Retrieve traces for the given transaction(s).`

var notesTraces = `
Notes:
  - The transactions list may be one or more space-separated identifiers which are either a transaction hash,
    a blockNumber.transactionID pair, or a blockHash.transactionID pair, or any combination.
  - This tool checks for valid input syntax, but does not check that the transaction requested actually exists.
  - If the queried node does not store historical state, the results for most older transactions are undefined.
  - A bang seperated filter has the following fields (at least one of which is required) and is separated
    with a bang (!): fromBlk, toBlk, fromAddr, toAddr, after, count.
  - A state diff trace describes, for each modified address, what changed during that trace.`

type tracesOptionsType struct {
	articulate bool
	filter     string
	statediff  bool
	count      bool
	skip_ddos  bool
	max        uint64
}

var TracesOpts tracesOptionsType

func init() {
	tracesCmd.SetOut(os.Stderr)

	tracesCmd.Flags().SortFlags = false
	tracesCmd.PersistentFlags().SortFlags = false
	tracesCmd.Flags().BoolVarP(&TracesOpts.articulate, "articulate", "a", false, "articulate the retrieved data if ABIs can be found")
	tracesCmd.Flags().StringVarP(&TracesOpts.filter, "filter", "f", "", "call the node's trace_filter routine with bang-seperated filter")
	tracesCmd.Flags().BoolVarP(&TracesOpts.statediff, "statediff", "d", false, "export state diff traces (not implemented)")
	tracesCmd.Flags().BoolVarP(&TracesOpts.count, "count", "c", false, "show the number of traces for the transaction only (fast)")
	tracesCmd.Flags().BoolVarP(&TracesOpts.skip_ddos, "skip_ddos", "s", false, "skip over the 2016 ddos during export ('on' by default) (hidden)")
	tracesCmd.Flags().Uint64VarP(&TracesOpts.max, "max", "m", 0, "if --skip_ddos is on, this many traces defines what a ddos transaction is (default = 250) (hidden)")
	if IsTestMode() == false {
		tracesCmd.Flags().MarkHidden("skip_ddos")
		tracesCmd.Flags().MarkHidden("max")
	}
	tracesCmd.Flags().SortFlags = false
	tracesCmd.PersistentFlags().SortFlags = false

	tracesCmd.SetUsageTemplate(HelpWithNotes(notesTraces))
	rootCmd.AddCommand(tracesCmd)
}

func runTraces(cmd *cobra.Command, args []string) {
	options := ""
	if TracesOpts.articulate {
		options += " --articulate"
	}
	if len(TracesOpts.filter) > 0 {
		options += " --filter " + TracesOpts.filter
	}
	if TracesOpts.statediff {
		options += " --statediff"
	}
	if TracesOpts.count {
		options += " --count"
	}
	if TracesOpts.skip_ddos {
		options += " --skip_ddos"
	}
	if TracesOpts.max > 0 {
		options += " --max " + fmt.Sprintf("%d", TracesOpts.max)
	}
	arguments := ""
	for _, arg := range args {
		arguments += " " + arg
	}
	PassItOn("/Users/jrush/.local/bin/chifra/getTraces", options, arguments)
}

func validateTracesArgs(cmd *cobra.Command, args []string) error {
	if len(args) > 0 && args[0] == "12" {
		return ErrFunc(cmd, errors.New("Invalid argument "+args[0]))
	}
	return nil
}
