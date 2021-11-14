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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/cmd/globals"
	receiptsPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/receipts"
	"github.com/spf13/cobra"
)

// receiptsCmd represents the receipts command
var receiptsCmd = &cobra.Command{
	Use:   usageReceipts,
	Short: shortReceipts,
	Long:  longReceipts,
	RunE:  receiptsPkg.Run,
	Args:  receiptsPkg.Validate,
}

var usageReceipts = `receipts [flags] <tx_id> [tx_id...]

Arguments:
  transactions - a space-separated list of one or more transaction identifiers (required)`

var shortReceipts = "retrieve receipts for the given transaction(s)"

var longReceipts = `Purpose:
  Retrieve receipts for the given transaction(s).`

var notesReceipts = `
Notes:
  - The transactions list may be one or more transaction hashes, blockNumber.transactionID pairs, or a blockHash.transactionID pairs.
  - This tool checks for valid input syntax, but does not check that the transaction requested actually exists.
  - If the queried node does not store historical state, the results for most older transactions are undefined.`

func init() {
	receiptsCmd.Flags().SortFlags = false
	receiptsCmd.Flags().BoolVarP(&receiptsPkg.Options.Articulate, "articulate", "a", false, "articulate the retrieved data if ABIs can be found")
	receiptsCmd.Flags().SortFlags = false
	globals.GlobalOptions(receiptsCmd, &receiptsPkg.Options.Globals)

	receiptsCmd.SetUsageTemplate(UsageWithNotes(notesReceipts))
	receiptsCmd.SetOut(os.Stderr)
	if receiptsPkg.Options.Globals.ApiMode {
		receiptsCmd.SetErr(os.Stdout)
	}
	chifraCmd.AddCommand(receiptsCmd)
}
