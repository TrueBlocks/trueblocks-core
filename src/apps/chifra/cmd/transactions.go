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

// transactionsCmd represents the transactions command
var transactionsCmd = &cobra.Command{
	Use:   usageTransactions,
	Short: shortTransactions,
	Long:  longTransactions,
	Run:   runTransactions,
	Args:  ValidatePositionals(validateTransactionsArgs),
}

var usageTransactions = `transactions [flags] <tx_id> [tx_id...]

Arguments:
  transactions - a space-separated list of one or more transaction identifiers (required)`

var shortTransactions = "retrieve one or more transactions from the chain or local cache"

var longTransactions = `Purpose:
  Retrieve one or more transactions from the chain or local cache.`

var notesTransactions = `
Notes:
  - The transactions list may be one or more space-separated identifiers which are either a transaction hash,
    a blockNumber.transactionID pair, or a blockHash.transactionID pair, or any combination.
  - This tool checks for valid input syntax, but does not check that the transaction requested actually exists.
  - If the queried node does not store historical state, the results for most older transactions are undefined.`

type transactionsOptionsType struct {
	articulate bool
	trace      bool
	uniq       bool
	reconcile  string
	cache      bool
}

var TransactionsOpts transactionsOptionsType

func init() {
	transactionsCmd.SetOut(os.Stderr)

	transactionsCmd.Flags().SortFlags = false
	transactionsCmd.PersistentFlags().SortFlags = false
	transactionsCmd.Flags().BoolVarP(&TransactionsOpts.articulate, "articulate", "a", false, "articulate the retrieved data if ABIs can be found")
	transactionsCmd.Flags().BoolVarP(&TransactionsOpts.trace, "trace", "t", false, "include the transaction's traces in the results")
	transactionsCmd.Flags().BoolVarP(&TransactionsOpts.uniq, "uniq", "u", false, "display a list of uniq addresses found in the transaction instead of the underlying data")
	transactionsCmd.Flags().StringVarP(&TransactionsOpts.reconcile, "reconcile", "r", "", "reconcile the transaction as per the provided address")
	transactionsCmd.Flags().BoolVarP(&TransactionsOpts.cache, "cache", "o", false, "force the results of the query into the tx cache (and the trace cache if applicable)")
	transactionsCmd.Flags().SortFlags = false
	transactionsCmd.PersistentFlags().SortFlags = false

	transactionsCmd.SetUsageTemplate(HelpWithNotes(notesTransactions))
	rootCmd.AddCommand(transactionsCmd)
}

func runTransactions(cmd *cobra.Command, args []string) {
	options := ""
	if TransactionsOpts.articulate {
		options += " --articulate"
	}
	if TransactionsOpts.trace {
		options += " --trace"
	}
	if TransactionsOpts.uniq {
		options += " --uniq"
	}
	if len(TransactionsOpts.reconcile) > 0 {
		options += " --reconcile " + TransactionsOpts.reconcile
	}
	if TransactionsOpts.cache {
		options += " --cache"
	}
	arguments := ""
	for _, arg := range args {
		arguments += " " + arg
	}
	// EXISTING_CODE
	// EXISTING_CODE
	PassItOn(GetCommandPath("getTrans"), options, arguments)
}

func validateTransactionsArgs(cmd *cobra.Command, args []string) error {
	var err error
	// EXISTING_CODE
	// EXISTING_CODE
	err = validateGlobalFlags()
	if err != nil {
		return err
	}
	return nil
}
