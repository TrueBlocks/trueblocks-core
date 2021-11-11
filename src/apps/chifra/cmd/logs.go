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

	logsPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/cmd/logs"
	"github.com/spf13/cobra"
)

// logsCmd represents the logs command
var logsCmd = &cobra.Command{
	Use:   usageLogs,
	Short: shortLogs,
	Long:  longLogs,
	RunE:  logsPkg.Run,
	Args:  logsPkg.Validate,
}

var usageLogs = `logs [flags] <tx_id> [tx_id...]

Arguments:
  transactions - a space-separated list of one or more transaction identifiers`

var shortLogs = "retrieve logs for the given transaction(s)"

var longLogs = `Purpose:
  Retrieve logs for the given transaction(s).`

var notesLogs = `
Notes:
  - The transactions list may be one or more transaction hashes, blockNumber.transactionID pairs, or a blockHash.transactionID pairs.
  - This tool checks for valid input syntax, but does not check that the transaction requested actually exists.
  - If the queried node does not store historical state, the results for most older transactions are undefined.
  - If you specify a 32-byte hash, it will be assumed to be a transaction hash, if the transaction is not found, it will be used as a topic.`

func init() {
	logsCmd.SetOut(os.Stderr)

	logsCmd.Flags().SortFlags = false
	logsCmd.PersistentFlags().SortFlags = false
	logsCmd.Flags().BoolVarP(&logsPkg.Options.Articulate, "articulate", "a", false, "articulate the retrieved data if ABIs can be found")
	logsCmd.Flags().SortFlags = false
	logsCmd.PersistentFlags().SortFlags = false

	logsCmd.SetUsageTemplate(UsageWithNotes(notesLogs))
	rootCmd.AddCommand(logsCmd)
}
