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

type logsOptionsType struct {
	topic      string
	source     string
	articulate bool
}

var LogsOpts logsOptionsType

// logsCmd represents the logs command
var logsCmd = &cobra.Command{
	Use: `logs [flags] <tx_id> [tx_id...]

Arguments:
  transactions - a space-separated list of one or more transaction identifiers (required)`,
	Short: "retrieve logs for the given transaction(s)",
	Long: `Purpose:
  Retrieve logs for the given transaction(s).`,
	Run: runLogs,
}

func init() {
	logsCmd.Flags().SortFlags = false
	logsCmd.PersistentFlags().SortFlags = false
	logsCmd.SetOut(os.Stderr)

	// logsCmd.Flags().ListtopicVarP(&LogsOpts.topic, "topic", "t", false, "filter by one or more log topics (not implemented)")
	// logsCmd.Flags().ListaddrVarP(&LogsOpts.source, "source", "s", false, "export only if the given address emitted the event (not implemented)")
	logsCmd.Flags().BoolVarP(&LogsOpts.articulate, "articulate", "a", false, "articulate the retrieved data if ABIs can be found")

	rootCmd.AddCommand(logsCmd)
}

func runLogs(cmd *cobra.Command, args []string) {
	options := ""
	// if LogsOpts.topic {
	// 	options += " --topic"
	// }
	// if LogsOpts.source {
	// 	options += " --source"
	// }
	if LogsOpts.articulate {
		options += " --articulate"
	}
	for _, arg := range args {
		options += " " + arg
	}
	PassItOn("/Users/jrush/.local/bin/chifra/getLogs", options, strconv.FormatUint(0, 10))
}
