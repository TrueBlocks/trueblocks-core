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

type tracesOptionsType struct {
	articulate bool
	filter     string
	statediff  bool
	count      bool
	skip_ddos  bool
	max        uint64
}

var TracesOpts tracesOptionsType

// tracesCmd represents the traces command
var tracesCmd = &cobra.Command{
	Use: `traces [flags] <tx_id> [tx_id...]

Arguments:
  transactions - a space-separated list of one or more transaction identifiers (required)`,
	Short: "retrieve traces for the given transaction(s)",
	Long: `Purpose:
  Retrieve traces for the given transaction(s).`,
	Run: runTraces,
}

func init() {
	tracesCmd.Flags().SortFlags = false
	tracesCmd.PersistentFlags().SortFlags = false
	tracesCmd.SetOut(os.Stderr)

	tracesCmd.Flags().BoolVarP(&TracesOpts.articulate, "articulate", "a", false, "articulate the retrieved data if ABIs can be found")
	tracesCmd.Flags().StringVarP(&TracesOpts.filter, "filter", "f", "", "call the node's trace_filter routine with bang-seperated filter")
	tracesCmd.Flags().BoolVarP(&TracesOpts.statediff, "statediff", "d", false, "export state diff traces (not implemented)")
	tracesCmd.Flags().BoolVarP(&TracesOpts.count, "count", "c", false, "show the number of traces for the transaction only (fast)")
	tracesCmd.Flags().BoolVarP(&TracesOpts.skip_ddos, "skip_ddos", "s", false, "skip over the 2016 ddos during export ('on' by default)")
	tracesCmd.Flags().Uint64VarP(&TracesOpts.max, "max", "m", 0, "if --skip_ddos is on, this many traces defines what a ddos transaction is (default = 250)")

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
		options += " --max " + strconv.FormatUint(TracesOpts.max, 10)
	}
	for _, arg := range args {
		options += " " + arg
	}
	PassItOn("/Users/jrush/.local/bin/chifra/getTraces", options, strconv.FormatUint(0, 10))
}
