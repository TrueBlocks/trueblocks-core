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
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

// EXISTING_CODE
import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	transactionsPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/transactions"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// transactionsCmd represents the transactions command
var transactionsCmd = &cobra.Command{
	Use:     usageTransactions,
	Short:   shortTransactions,
	Long:    longTransactions,
	Version: "GHC-TrueBlocks//0.16.1-alpha",
	RunE:    transactionsPkg.RunTransactions,
}

var usageTransactions = `transactions [flags] <tx_id> [tx_id...]

Arguments:
  transactions - a space-separated list of one or more transaction identifiers (required)`

var shortTransactions = "retrieve one or more transactions from the chain or local cache"

var longTransactions = `Purpose:
  Retrieve one or more transactions from the chain or local cache.`

var notesTransactions = `
Notes:
  - The transactions list may be one or more transaction hashes, blockNumber.transactionID pairs, or a blockHash.transactionID pairs.
  - This tool checks for valid input syntax, but does not check that the transaction requested actually exists.
  - If the queried node does not store historical state, the results for most older transactions are undefined.`

func init() {
	transactionsCmd.Flags().SortFlags = false

	transactionsCmd.Flags().BoolVarP(&transactionsPkg.Options.Articulate, "articulate", "a", false, "articulate the retrieved data if ABIs can be found")
	transactionsCmd.Flags().BoolVarP(&transactionsPkg.Options.Trace, "trace", "t", false, "include the transaction's traces in the results")
	transactionsCmd.Flags().BoolVarP(&transactionsPkg.Options.Uniq, "uniq", "u", false, "display a list of uniq addresses found in the transaction")
	transactionsCmd.Flags().StringVarP(&transactionsPkg.Options.Reconcile, "reconcile", "r", "", "reconcile the transaction as per the provided address")
	transactionsCmd.Flags().BoolVarP(&transactionsPkg.Options.Cache, "cache", "o", false, "force the results of the query into the tx cache (and the trace cache if applicable)")
	globals.InitGlobals(transactionsCmd, &transactionsPkg.Options.Globals)

	transactionsCmd.SetUsageTemplate(UsageWithNotes(notesTransactions))
	transactionsCmd.SetOut(os.Stderr)

	chifraCmd.AddCommand(transactionsCmd)
}
