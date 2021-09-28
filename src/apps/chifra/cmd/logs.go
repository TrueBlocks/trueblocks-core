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
 * Parts of this file were generated with makeClass --gocmds. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

import (
	// EXISTING_CODE
	"os"

	"github.com/spf13/cobra"
	// EXISTING_CODE
)

// logsCmd represents the logs command
var logsCmd = &cobra.Command{
	Use:   usageLogs,
	Short: shortLogs,
	Long:  longLogs,
	Run:   runLogs,
	Args:  validateLogsArgs,
}

var usageLogs = `logs [flags] <tx_id> [tx_id...]

Arguments:
  transactions - a space-separated list of one or more transaction identifiers (required)`

var shortLogs = "retrieve logs for the given transaction(s)"

var longLogs = `Purpose:
  Retrieve logs for the given transaction(s).`

var notesLogs = `
Notes:
  - The transactions list may be one or more space-separated identifiers which are either a transaction hash,
    a blockNumber.transactionID pair, or a blockHash.transactionID pair, or any combination.
  - This tool checks for valid input syntax, but does not check that the transaction requested actually exists.
  - If the queried node does not store historical state, the results for most older transactions are undefined.
  - If you specify a 32-byte hash, it will be assumed to be a transaction hash, if the transaction is
    not found, it will be used as a topic.`

type logsOptionsType struct {
	topic      []string
	source     []string
	articulate bool
}

var LogsOpts logsOptionsType

func init() {
	logsCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE
	logsCmd.Flags().SortFlags = false
	logsCmd.PersistentFlags().SortFlags = false
	logsCmd.Flags().StringSliceVarP(&LogsOpts.topic, "topic", "t", nil, "filter by one or more log topics (not implemented)")
	logsCmd.Flags().StringSliceVarP(&LogsOpts.source, "source", "s", nil, "export only if the given address emitted the event (not implemented)")
	logsCmd.Flags().BoolVarP(&LogsOpts.articulate, "articulate", "a", false, "articulate the retrieved data if ABIs can be found")
	logsCmd.Flags().SortFlags = false
	logsCmd.PersistentFlags().SortFlags = false
	// EXISTING_CODE
	// EXISTING_CODE

	logsCmd.SetUsageTemplate(HelpWithNotes(notesLogs))
	rootCmd.AddCommand(logsCmd)
}

func runLogs(cmd *cobra.Command, args []string) {
	options := ""
	for _, t := range LogsOpts.topic {
		options += " --topic " + t
	}
	for _, t := range LogsOpts.source {
		options += " --source " + t
	}
	if LogsOpts.articulate {
		options += " --articulate"
	}
	arguments := ""
	for _, arg := range args {
		arguments += " " + arg
	}
	// EXISTING_CODE
	// EXISTING_CODE
	PassItOn(GetCommandPath("getLogs"), options, arguments)
}

// EXISTING_CODE
// EXISTING_CODE

func validateLogsArgs(cmd *cobra.Command, args []string) error {
	var err error
	// EXISTING_CODE
	// EXISTING_CODE
	err = validateGlobalFlags(cmd, args)
	if err != nil {
		return err
	}
	return nil
}
