// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package cmd

// EXISTING_CODE
import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/internal/globals"
	receiptsPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/internal/receipts"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/file"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// receiptsCmd represents the receipts command
var receiptsCmd = &cobra.Command{
	Use:     usageReceipts,
	Long:    longReceipts,
	Version: versionText,
	PreRun: outputHelpers.PreRunWithJsonWriter("receipts", func() *globals.GlobalOptions {
		return &receiptsPkg.GetOptions().Globals
	}),
	RunE: file.RunWithFileSupport("receipts", receiptsPkg.RunReceipts, receiptsPkg.ResetOptions),
	PostRun: outputHelpers.PostRunWithJsonWriter(func() *globals.GlobalOptions {
		return &receiptsPkg.GetOptions().Globals
	}),
}

const usageReceipts = `receipts [flags] <tx_id> [tx_id...]

Arguments:
  transactions - a space-separated list of one or more transaction identifiers (required)`

const longReceipts = `Purpose:
  Retrieve receipts for the given transaction(s).`

const notesReceipts = `
Notes:
  - The transactions list may be one or more transaction hashes, blockNumber.transactionID pairs, or a blockHash.transactionID pairs.
  - This tool checks for valid input syntax, but does not check that the transaction requested actually exists.
  - If the queried node does not store historical state, the results for most older transactions are undefined.`

func init() {
	var capabilities caps.Capability // capabilities for chifra receipts
	capabilities = capabilities.Add(caps.Default)
	capabilities = capabilities.Add(caps.Caching)

	receiptsCmd.Flags().SortFlags = false

	receiptsCmd.Flags().BoolVarP(&receiptsPkg.GetOptions().Articulate, "articulate", "a", false, `articulate the retrieved data if ABIs can be found`)
	globals.InitGlobals("receipts", receiptsCmd, &receiptsPkg.GetOptions().Globals, capabilities)

	receiptsCmd.SetUsageTemplate(UsageWithNotes(notesReceipts))
	receiptsCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand(receiptsCmd)
}
