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
	transactionsPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/transactions"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// transactionsCmd represents the transactions command
var transactionsCmd = &cobra.Command{
	Use:     usageTransactions,
	Short:   shortTransactions,
	Long:    longTransactions,
	Version: "GHC-TrueBlocks//0.18.0-alpha",
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
