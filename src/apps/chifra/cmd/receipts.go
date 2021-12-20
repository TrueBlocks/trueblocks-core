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
	receiptsPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/receipts"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// receiptsCmd represents the receipts command
var receiptsCmd = &cobra.Command{
	Use:     usageReceipts,
	Short:   shortReceipts,
	Long:    longReceipts,
	Version: versionText,
	RunE:    receiptsPkg.RunReceipts,
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
	globals.InitGlobals(receiptsCmd, &receiptsPkg.Options.Globals)

	receiptsCmd.SetUsageTemplate(UsageWithNotes(notesReceipts))
	receiptsCmd.SetOut(os.Stderr)

	chifraCmd.AddCommand(receiptsCmd)
}
