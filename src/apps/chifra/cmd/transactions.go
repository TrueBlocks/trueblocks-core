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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// transactionsCmd represents the transactions command
var transactionsCmd = &cobra.Command{
	Use:     usageTransactions,
	Short:   shortTransactions,
	Long:    longTransactions,
	Version: versionText,
	PreRun: outputHelpers.PreRunWithJsonWriter("transactions", func() *globals.GlobalOptions {
		return &transactionsPkg.GetOptions().Globals
	}),
	RunE: file.RunWithFileSupport("transactions", transactionsPkg.RunTransactions, transactionsPkg.ResetOptions),
	PostRun: outputHelpers.PostRunWithJsonWriter(func() *globals.GlobalOptions {
		return &transactionsPkg.GetOptions().Globals
	}),
}

const usageTransactions = `transactions [flags] <tx_id> [tx_id...]

Arguments:
  transactions - a space-separated list of one or more transaction identifiers (required)`

const shortTransactions = "retrieve one or more transactions from the chain or local cache"

const longTransactions = `Purpose:
  Retrieve one or more transactions from the chain or local cache.`

const notesTransactions = `
Notes:
  - The transactions list may be one or more transaction hashes, blockNumber.transactionID pairs, or a blockHash.transactionID pairs.
  - This tool checks for valid input syntax, but does not check that the transaction requested actually exists.
  - If the queried node does not store historical state, the results for most older transactions are undefined.
  - The --traces option, when used with --account_for, will descend into traces to complete reconciliations.
  - The --decache option removes the all transaction(s) and all traces in those transactions from the cache.`

func init() {
	var capabilities = caps.Default // Additional global caps for chifra transactions
	// EXISTING_CODE
	capabilities = capabilities.Add(caps.Caching)
	capabilities = capabilities.Add(caps.Raw)
	capabilities = capabilities.Add(caps.Ether)
	capabilities = capabilities.Add(caps.Wei)
	// EXISTING_CODE

	transactionsCmd.Flags().SortFlags = false

	transactionsCmd.Flags().BoolVarP(&transactionsPkg.GetOptions().Articulate, "articulate", "a", false, "articulate the retrieved data if ABIs can be found")
	transactionsCmd.Flags().BoolVarP(&transactionsPkg.GetOptions().Traces, "traces", "t", false, "include the transaction's traces in the results")
	transactionsCmd.Flags().BoolVarP(&transactionsPkg.GetOptions().Uniq, "uniq", "u", false, "display a list of uniq addresses found in the transaction")
	transactionsCmd.Flags().StringVarP(&transactionsPkg.GetOptions().Flow, "flow", "f", "", `for the uniq option only, export only from or to (including trace from or to)
One of [ from | to ]`)
	transactionsCmd.Flags().BoolVarP(&transactionsPkg.GetOptions().Logs, "logs", "l", false, "display only the logs found in the transaction(s)")
	transactionsCmd.Flags().StringSliceVarP(&transactionsPkg.GetOptions().Emitter, "emitter", "m", nil, "for the --logs option only, filter logs to show only those logs emitted by the given address(es)")
	transactionsCmd.Flags().StringSliceVarP(&transactionsPkg.GetOptions().Topic, "topic", "B", nil, "for the --logs option only, filter logs to show only those with this topic(s)")
	transactionsCmd.Flags().StringVarP(&transactionsPkg.GetOptions().AccountFor, "account_for", "A", "", "reconcile the transaction as per the provided address")
	transactionsCmd.Flags().BoolVarP(&transactionsPkg.GetOptions().CacheTraces, "cache_traces", "", false, "force the transaction's traces into the cache (hidden)")
	transactionsCmd.Flags().BoolVarP(&transactionsPkg.GetOptions().Source, "source", "s", false, "find the source of the funds sent to the receiver (hidden)")
	if os.Getenv("TEST_MODE") != "true" {
		transactionsCmd.Flags().MarkHidden("cache_traces")
		transactionsCmd.Flags().MarkHidden("source")
	}
	globals.InitGlobals(transactionsCmd, &transactionsPkg.GetOptions().Globals, capabilities)

	transactionsCmd.SetUsageTemplate(UsageWithNotes(notesTransactions))
	transactionsCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand(transactionsCmd)
}
